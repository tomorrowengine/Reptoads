#define WIN32_LEAN_AND_MEAN
#include <algorithm>
#include <utility>
#include "GameServer.h"
#include "LoggingFunction.h"
#include "Net/Server.h"
#include "core/StrHash.h"
#include "Payloads.h"
#include "net/LobbyCommands.h"
#include "Net/GameCommands.h"
#include "resource_loading/ConfigLoader.h"
#include "utils/CMDInput.h"
#include "Crypto/NetRSA.h"
#include "Net/Packet.h"
#include "resource_loading/ArgumentParsing.h"
#include "gameplay/MatchHelper.h"
#include "gameplay/GamePlayHelper.h"

#ifndef _WIN32
#define Sleep usleep
#endif

tbsg::GameServer::GameServer(cof::basic_logger::Logger* logger, std::string hostname) : Server(logger), hostname(std::move(hostname))
{
}

tbsg::MatchProfileType tbsg::GameServer::GetMatchProfileType(net::Connection* connection)
{
	const auto profile = profileDatabase.GetProfileUsingConnection(connection->GetConnectionId());
	if (connection->GetConnectionId() == 0) std::terminate();
	if (profile)
	{
		return GetMatchProfileType(profile);
	}
	return MatchProfileType::Unknown;
}

tbsg::MatchProfileType tbsg::GameServer::GetMatchProfileType(Profile* profile)
{
	if (profile->id == currentMatch.ownerProfileId)
	{
		return MatchProfileType::Owner;
	}
	if (profile->id == currentMatch.opponentProfileId)
	{
		return MatchProfileType::Opponent;
	}
	return MatchProfileType::Unknown;
}

void tbsg::GameServer::UpdateConsoleTitle()
{
#ifdef _WIN32
	SetConsoleTitleA(("GameServer " + std::to_string(serverId) + " (" + tbsg::Config::Get().address + ":" + std::to_string(GetPort()) + ") (Connections: " + std::to_string(GetConnections().size()) + ") " + "(" + (currentMatch.id == 0 ? "Inactive" : "Active") + ")").c_str());
#endif
}

tbsg::GameServer::~GameServer()
{
	for (auto handle : listenerHandles)
	{
		serverEQ->removeListener(handle.first, handle.second);
	}
}

void tbsg::GameServer::Initialize(int argc, char* argv[])
{
	this->SetDebug(true);
	cof::Info("{} Initializing GameServer", prefix);

	tbsg::LoadConfig("./config.json");
	Config& config = Config::Get();
	ParsedArguments arguments;

	config = ArgumentParsing(config, argc, argv, arguments);
	if (config.gameServerAutoPortIncrement)
	{
		config.port = config.port + 1;
	}

	db::DatabaseCredentials credentials;
	credentials.host = config.databaseHost;
	credentials.username = config.databaseUsername;
	credentials.password = config.databasePassword;
	credentials.database = config.databaseName;
	credentials.port = config.databasePort;
	this->api = db::DatabaseAPI{ credentials, config.projectId };
	this->api.OpenDatabaseConnection();

	this->profileDatabase.Initialize(&this->api);
	this->gameDataDatabase.Initialize(&this->api);

	
	this->gameDataDatabase.CreateHarddriveCopyOfScript("CombatLibrary");

	this->InitializeGameplay();

	if (config.simulateGame)
	{
		ptl::vector<ptl::string> allArguments(argv + 1, argv + argc);
		this->RunGameSimulation(allArguments);
	}
	else
	{
		this->StartServer(config.port, config.maxConnection);
		this->InitializeQueue();
	}

	this->serverId = this->api.RegisterServer(config.GetAddress(), config.GetPort(), this->hostname, tbsg::Config::Get().IsProduction());

}

void tbsg::GameServer::InitializeQueue()
{
	listenerHandles.insert(std::pair<ClientGameCommands, ServerEventQueue::Handle>{ClientGameCommands::ClientCanStart,
		serverEQ->appendListener(ClientGameCommands::ClientCanStart,
			[this](Packet&& packet, net::Connection* connection) { this->ClientCanStart(std::move(packet), connection); })
	}
	);

	listenerHandles.insert(std::pair<ClientGameCommands, ServerEventQueue::Handle>{ClientGameCommands::OnCardHover,
		serverEQ->appendListener(ClientGameCommands::OnCardHover,
			[this](Packet&& packet, net::Connection* connection) { this->CardHover(std::move(packet), connection); })
	}
	);

	listenerHandles.insert(std::pair<ClientGameCommands, ServerEventQueue::Handle>{ClientGameCommands::OnCardSelect,
		serverEQ->appendListener(ClientGameCommands::OnCardSelect,
			[this](Packet&& packet, net::Connection* connection) { this->CardSelect(std::move(packet), connection); })
	}
	);

	listenerHandles.insert(std::pair<ClientGameCommands, ServerEventQueue::Handle>{ClientGameCommands::OnCardsSubmit,
		serverEQ->appendListener(ClientGameCommands::OnCardsSubmit,
			[this](Packet&& packet, net::Connection* connection) { this->CardsSubmit(std::move(packet), connection); })
	}
	);
}

void tbsg::GameServer::Run()
{
	CMDInput cli;
	cli.Run();

	auto lastOnlineUpdate = std::chrono::high_resolution_clock::now();

	auto last = std::chrono::system_clock::now();
	while (cli.IsRunning())
	{
		ReceivePackets(); 
		HandlePackets();
		cli.HandleParsedAndHandleCommands(this);
		serverEQ->process();

		auto nowOnlineUpdate = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(nowOnlineUpdate - lastOnlineUpdate).count() > 5)
		{
			api.PingServer(this->serverId);
			this->UpdateConsoleTitle();
			lastOnlineUpdate = nowOnlineUpdate;
		}

		auto now = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed = now - last;
		double untilNextRun = 20 - elapsed.count();
		if (untilNextRun > 0)
		{
			Sleep(untilNextRun);
		}
		last = now;
	}

	cli.Release();
}

net::IdentifyResponse tbsg::GameServer::IdentifyClient(Packet& packet, net::Connection* connection)
{
	ptl::string token{};
	packet >> token;
	auto response = this->profileDatabase.CheckSessionToken(connection, token);
	if (response == tbsg::SessionTokenResponse::Success)
	{
		auto profile = this->profileDatabase.GetProfile(connection);
		cof::Info("{} Connection {} has successfully identified itself as {}.", prefix, connection->GetConnectionId(), profile->username);
		return net::IdentifyResponse::Success;
	}

	if (!token.empty())
	{
		cof::Info("{} Connection {} failed to provide a valid identity. Received token: {}", prefix, connection->GetConnectionId(), token);
	}
	else
	{
		cof::Info("{} Connection {} failed to provide a valid identity.", prefix, connection->GetConnectionId());
	}

	if (response == tbsg::SessionTokenResponse::InUse)
	{
		return net::IdentifyResponse::AccountInUse;
	}
	return net::IdentifyResponse::Invalid;
}

void tbsg::GameServer::HandleCustomPacket(unsigned customCommand, Packet& packet, net::Connection* connection)
{
	auto command = ClientGameCommands(customCommand);
	if (IsDebug())
	{
		cof::Debug("{} Handling custom packet: ClientCommands::{} ({})", prefix, GetName(command), customCommand);
	}
	switch (command)
	{
	case ClientGameCommands::ClientCanStart:
		serverEQ->enqueue(ClientGameCommands::ClientCanStart, std::move(packet), connection);
		break;
	case ClientGameCommands::OnCardHover:
		serverEQ->enqueue(ClientGameCommands::OnCardHover, std::move(packet), connection);
		break;
	case ClientGameCommands::OnCardSelect:
		serverEQ->enqueue(ClientGameCommands::OnCardSelect, std::move(packet), connection);
		break;
	case ClientGameCommands::OnCardsSubmit:
		serverEQ->enqueue(ClientGameCommands::OnCardsSubmit, std::move(packet), connection);
		break;
	}
}

void tbsg::GameServer::OnPlayerDisconnected(net::Connection* connection)
{
	const auto profile = this->profileDatabase.GetProfile(connection);
	if (profile)
	{
		cof::Info("{} Client {} ({}) disconnected", prefix, profile->username, connection->GetConnectionId());
		profile->connectionId = 0;
	}
	else
	{
		cof::Info("{} Client {} disconnected", prefix, connection->GetConnectionId());
	}

	if (this->firstConnection && connection->GetConnectionId() == this->firstConnection->GetConnectionId())
	{
		this->firstConnection = nullptr;
	}
	else if (this->secondConnection && connection->GetConnectionId() == this->secondConnection->GetConnectionId())
	{
		this->secondConnection = nullptr;
	}

	if (!this->firstConnection && !this->secondConnection)
	{
		this->InitializeGameplay();
		this->api.SetServerOccupied(this->serverId, false);
	}
}

void tbsg::GameServer::OnPlayerConnected(net::Connection* connection)
{

}

void tbsg::GameServer::OnPlayerIdentified(net::Connection* connection)
{
	tbsg::Profile* profile = this->profileDatabase.GetProfile(connection);
	this->api.RegisterIp(profile->id, NetUtils::EnetAddressToIpString(connection->GetPeer()->address));

	currentMatch = api.GetNextMatchForServer(serverId);

	
	if (firstConnection == nullptr)
	{
		firstConnection = connection;

		if(currentMatch.opponentProfileId == 0 && currentMatch.opponentDeckId == 0)
		{
			this->api.SetServerOccupied(this->serverId, true);
			this->SetupGame(currentMatch);
		}
	}
	else if (secondConnection == nullptr)
	{
		secondConnection = connection;
	}

	if (firstConnection != nullptr && secondConnection != nullptr)
	{
		
		this->api.SetServerOccupied(this->serverId, true);
		this->SetupGame(currentMatch);
		const auto sendData = [this](auto connection)
		{
			const MatchProfileType connection_ = this->GetMatchProfileType(connection);
			if (connection_ == MatchProfileType::Owner)
			{
				this->SendOverMatchData(connection, false);
			}
			else
			{
				this->SendOverMatchData(connection, true);
			}
		};

		sendData(firstConnection);
		sendData(secondConnection);

	}
}

std::string tbsg::GameServer::GetPlayerName(net::Connection* connection)
{
	if (connection)
	{
		auto profile = profileDatabase.GetProfile(connection);
		if (profile)
		{
			return profile->username;
		}
	}
	return std::string{};
}

void tbsg::GameServer::SendCustomPacket(ServerGameCommands command, net::Connection* connection)
{
	Packet emptyPacket{};
	this->SendCustomPacket(command, emptyPacket, connection);
}

void tbsg::GameServer::SendCustomPacket(ServerGameCommands command, Packet& packet, net::Connection* connection)
{
	if(connection == nullptr)
	{
		return;
	}
	auto commandId = static_cast<unsigned int>(command);
	if (IsDebug())
	{
		cof::Debug("{} Sending custom packet: ServerCommands::{} ({})", prefix, GetName(command), commandId);
	}
	Server::SendCustomPacket(commandId, packet, connection);
}

void tbsg::GameServer::InitializeGameplay()
{
	if (IsDebug())
	{
		cof::Debug("{} Initializing gameplay...", prefix);
	}

	if (luaContext.GetIsInitialized())
	{
		luaContext.Deallocate();
	}
	luaContext.Initialize();

	currentMatch = {};
	matchData = {};

	luaContext.Initialize();
	gameLogic.Initialize(luaContext);

	cof::Info("{} GameServer has been reset and is ready for the next game.", prefix);
}

void tbsg::GameServer::ClientCanStart(Packet&& packet, net::Connection* connection)
{
	if (connection == firstConnection)
	{
		gameLogic.firstPlayerReady = true;
	}
	else if (connection == secondConnection)
	{
		gameLogic.secondPlayerReady = true;
	}

	if (gameLogic.ArePlayersReady())
	{

		SendCustomPacket(ServerGameCommands::GameHasStarted, firstConnection);
		SendCustomPacket(ServerGameCommands::GameHasStarted, secondConnection);

		this->api.StartMatch(currentMatch.id);

		this->SetupPlayerCards();
	}
}

void tbsg::GameServer::CardHover(Packet&& packet, net::Connection* connection)
{
	SendCustomPacket(ServerGameCommands::OpponentOnCardHover, packet, DetermineDestinationConnection(connection));
}

void tbsg::GameServer::CardSelect(Packet&& packet, net::Connection* connection)
{
	SendCustomPacket(ServerGameCommands::OpponentOnCardSelect, packet, DetermineDestinationConnection(connection));
}

void tbsg::GameServer::CardsSubmit(Packet&& packet, net::Connection* connection)
{
	auto& connections = this->GetConnections();
	if (connections.size() == 2)
	{
		unsigned int amountCards = 0;
        unsigned int cardId = 0;
        packet >> amountCards;
		for (unsigned int i = 0; i < amountCards; ++i)
		{
			packet >> cardId;
			if (cardId != 0)
			{
				const auto firstConnection = connections[0].GetConnectionId() == connection->GetConnectionId();
				matchData.playedCards[firstConnection ? 0 : 1] = cardId;
			}
		}
		Packet newPacket;
		newPacket << amountCards;
        newPacket << cardId;
		SendCustomPacket(ServerGameCommands::OpponentOnCardsSubmit, newPacket, DetermineDestinationConnection(connection));

		if (matchData.playedCards[0] != 0 && matchData.playedCards[1] != 0)
		{
			this->RunOnPlay();

			matchData.playedCards[0] = 0;
			matchData.playedCards[1] = 0;
		}
	}
}

const unsigned int tbsg::GameServer::GetLobbyOwner()
{
	auto connections = GetConnections();
	MatchProfileType connection1 = GetMatchProfileType(&connections[0]);

	return connection1 == MatchProfileType::Owner ? 1 : 0;
}

net::Connection* tbsg::GameServer::DetermineDestinationConnection(net::Connection* connection)
{
	return connection == firstConnection ? secondConnection : firstConnection;
}

void tbsg::GameServer::TestEventFunc(unsigned int a )
{
	std::cout << "Value = " << a << std::endl;
}

void tbsg::GameServer::RunGameSimulation(ptl::vector<ptl::string>& allArguments)
{
	for (size_t i = 0; i < allArguments.size(); ++i)
	{
		auto& command = allArguments[i];
		if (command == "--simulate-game")
		{
			const bool hasTurnCount = vector_in_range(allArguments, i + 6);
			const bool hasSeed = vector_in_range(allArguments, i + 7);
			unsigned int seed{ 0 };
			unsigned int amountOfGamesToSimulate{ 1 };
			if (hasTurnCount)
			{
				amountOfGamesToSimulate = std::stoul(allArguments[i + 6]);
			}
			if (hasSeed)
			{
				seed = std::stoul(allArguments[i + 7]);
			}

			if (vector_in_range(allArguments, i + 5))
			{
				
				unsigned int player1ID = std::stoul(allArguments[i + 1]);
				unsigned int player2ID = std::stoul(allArguments[i + 2]);
				ptl::string monsterDeck = allArguments[i + 3];
				ptl::string player1Deck = allArguments[i + 4];
				ptl::string player2Deck = allArguments[i + 5];
				this->CreateGameDataFromDatabase(player1ID, player2ID, monsterDeck, player1Deck, player2Deck, seed);

				gameLogic.SimulateAIMatch(matchData, amountOfGamesToSimulate);
			}
			else
			{
				std::cout << "[RunGameSimulation] Amount expected arguments not valid, e.g: --simulate-game 1  2  MonsterDeck1  Deck2  Deck5" << std::endl;
			}
		}
	}
}


void tbsg::GameServer::RunOnPlay()
{
	auto result = tbsg::gameplay::OnPlay(matchData, luaContext, &gameDataDatabase);
	if (!result.results.empty())
	{
		Packet firstPacket = this->CreatePacketForEffectChanges(result);
		Packet secondPacket = this->CreatePacketForEffectChangesWithSwap(result);
		auto connections = GetConnections();

		unsigned int ownerID = GetLobbyOwner();

		this->SendCustomPacket(ServerGameCommands::TurnResult, firstPacket, &connections[ownerID]);
		this->SendCustomPacket(ServerGameCommands::TurnResult, secondPacket, &connections[!ownerID]);
	}
}

void tbsg::GameServer::CreateGameDataFromDatabase(unsigned player1Id, unsigned player2Id, const ptl::string& monsterDeckName, const ptl::string& player1DeckName, const
	ptl::string& player2DeckName, unsigned int seed)
{
	
	auto validGame = true;

	const auto LoadProfile = [this](auto id) -> bool
	{
		if (this->profileDatabase.LoadProfile(id))
		{
			auto profile = this->profileDatabase.GetProfile(id);
			if (profile != nullptr)
			{
				return true;
			}
			else
			{
				cof::Error("[RunGameSimulation] player profile 1 is not found, requested id: {}", id);
				return false;
			}
		}
		else
		{
			cof::Error("[RunGameSimulation] player profile 1 is not found, requested id: {}", id);
			return false;

		}
	};

	const auto LoadDeckOfProfile = [this, &validGame](auto id, auto deckName) -> auto&
	{
		gameDataDatabase.LoadDecksOfProfile(id);
		tbsg::Deck* deck{ nullptr };
		const unsigned int deckId = std::atoi(deckName.c_str());
		if (deckId != 0)
		{
			deck = this->GetDeckBasedOnID(id, deckId);
		}
		else
		{
			deck = this->GetDeckBasedOnString(id, deckName);
		}
		if (deck == nullptr)
		{
			validGame = false;
			cof::Error("[RunGameSimulation] player1Deck is nullptr, requested id: {} with deck name: {} ", id, deckName);
		}

		return deck;
	};

	const auto CheckGame = [&validGame]
	{
		if (!validGame)
		{
			
			return false;
		}
		return true;
	};

	validGame = LoadProfile(player1Id);
	validGame = LoadProfile(player2Id);

	if (!CheckGame()) return;

	
	tbsg::MonsterDeck monsterDeck{};

	if (this->gameDataDatabase.LoadMonsterDeck(monsterDeckName))
	{
		const auto getMonsterDeck = this->gameDataDatabase.GetMonsterDeck(monsterDeckName);
		if (getMonsterDeck == nullptr)
		{
			validGame = false;
			cof::Error("[RunGameSimulation] monsterDeck is nullptr, requested name {}", monsterDeckName);
		}
		else
		{
			monsterDeck = *getMonsterDeck;
		}
	}
	else
	{
		validGame = false;
		cof::Error("[RunGameSimulation] monsterDeck is not found, requested name {}", monsterDeckName);
	}

	tbsg::Deck* player1Deck = LoadDeckOfProfile(player1Id, player1DeckName);
	tbsg::Deck* player2Deck = LoadDeckOfProfile(player2Id, player2DeckName);

	cof::Debug("[Game Server] Player 1 Deck: (Id: {}) {} Player 2 Deck: (Id: {}) {}", player1DeckName, player1Deck->name,player2DeckName, player2Deck->name);

	if (!CheckGame()) return;

	auto decks = ptl::vector<tbsg::Deck>{ *player1Deck, *player2Deck };
	const auto decks2Use = ptl::vector<ptl::vector<Card*>>{};

	matchData = tbsg::gameplay::CreateMatchData(monsterDeck, decks,
		decks2Use, ptl::vector<tbsg::Hero>{}, player1Id, player2Id, 2, 1, seed);

	cof::Debug("[Game Server] Match Data: Hand P1: {} P2: {}", matchData.playerHands[0].size(), matchData.playerHands[1].size());
}


void tbsg::GameServer::SetupGame(tbsg::Match& matchInformation)
{
	if (matchInformation.id != 0)
	{
		Packet packet;
		packet << matchInformation.id;
		packet << matchInformation.ownerProfileId;
		packet << matchInformation.ownerDeckId;
		packet << matchInformation.opponentProfileId;
		packet << matchInformation.opponentDeckId;
		packet << matchInformation.serverId;

		SendCustomPacket(ServerGameCommands::SendLobbyData, packet, firstConnection);
		if(secondConnection != nullptr)
		{
			SendCustomPacket(ServerGameCommands::SendLobbyData, packet, secondConnection);
		}

		const auto unix_timestamp = std::chrono::seconds(std::time(nullptr));
		auto& monsterDecks = gameDataDatabase.GetMonsterDecks();
		const unsigned int seed = std::chrono::milliseconds(unix_timestamp).count();
		auto mtSeed = std::mt19937{ seed };

		const auto index = tbsg::gameplay::GetRandomIntInRange(0, monsterDecks.size() - 1, mtSeed);		const ptl::string monsterDeck = monsterDecks.values.at(index).name;
		cof::Debug("[Game Server] Choose {} deck for monster", monsterDeck);
		const ptl::string player1Deck = ptl::to_string(matchInformation.ownerDeckId);
		const ptl::string player2Deck = ptl::to_string(matchInformation.opponentDeckId);

		cof::Debug("[Game Server] Owner Deck: {} Opponent Deck: {}", matchInformation.ownerDeckId, matchInformation.opponentDeckId);

		this->CreateGameDataFromDatabase(matchInformation.ownerProfileId, matchInformation.opponentProfileId, monsterDeck, player1Deck, player2Deck);
	}else
	{
		SendCustomPacket(ServerGameCommands::GameServerHasAnError, firstConnection);
		SendCustomPacket(ServerGameCommands::GameServerHasAnError, secondConnection);
	}
}

void tbsg::GameServer::SetupPlayerCards()
{

	for (auto& playerDeck : matchData.playerDecks)
	{
		gameplay::ShuffleCards(playerDeck);
	}

	ptl::vector<unsigned int> p1CardsAdded = tbsg::gameplay::DrawCards(matchData.playerDecks.at(0), matchData.playerHands.at(0), 3, 6);
	ptl::vector<unsigned int> p2CardsAdded = tbsg::gameplay::DrawCards(matchData.playerDecks.at(1), matchData.playerHands.at(1), 3, 6);
	tbsg::ResultOfRound changes;

	for (auto cardID : p1CardsAdded)
	{
		tbsg::Change change{ tbsg::EffectChange::Deck_DrawCard, static_cast<int>(cardID), 0 };
		changes.results.push_back(change);
	}
	for (auto cardID : p2CardsAdded)
	{
		tbsg::Change change{ tbsg::EffectChange::Deck_DrawCard, static_cast<int>(cardID), 1 };
		changes.results.push_back(change);
	}
	Packet firstPacket = this->CreatePacketForEffectChanges(changes);
	Packet secondPacket = this->CreatePacketForEffectChangesWithSwap(changes);
	auto connections = GetConnections();

	const unsigned int ownerID = GetLobbyOwner();
	this->SendCustomPacket(ServerGameCommands::TurnResult, firstPacket, &connections[ownerID]);
	this->SendCustomPacket(ServerGameCommands::TurnResult, secondPacket, &connections[!ownerID]);
}

void tbsg::GameServer::SendOverMatchData(net::Connection* connection, bool switched)
{
	const unsigned int player0Id = switched ? 1 : 0;
	const unsigned int player1Id = switched ? 0 : 1;

	
	Packet packet{};

	const unsigned int monsterCardsSize = matchData.monsterCards.size();
	packet << monsterCardsSize;
	for (unsigned int i = 0; i < monsterCardsSize; ++i)
	{
		
		packet << matchData.monsterCards.at(i).id;
		
		packet << matchData.monsterCards.at(i).meta.name;
		packet << matchData.monsterCards.at(i).meta.description;
		packet << matchData.monsterCards.at(i).meta.rarity;
		packet << matchData.monsterCards.at(i).meta.type;
		
		packet << matchData.monsterCards.at(i).data.health;
		packet << matchData.monsterCards.at(i).data.maxHealth;
		packet << matchData.monsterCards.at(i).data.armor;
		packet << matchData.monsterCards.at(i).data.monsterTrait;
		
		unsigned int rewardSize = matchData.monsterCards.at(i).data.reward.size();
		packet << rewardSize;
		for (unsigned int j = 0; j < rewardSize; ++j)
		{
			packet << static_cast<unsigned int>(matchData.monsterCards.at(j).data.reward.at(j)->type);
			packet << matchData.monsterCards.at(j).data.reward.at(j)->powerup;
		}
	}

	
	unsigned int playerDeck1Size = matchData.playerDecks.at(player0Id).size();
	packet << playerDeck1Size;
	for (unsigned int i = 0; i < playerDeck1Size; ++i)
	{
		packet << matchData.playerDecks.at(player0Id).at(i);
	}

	unsigned int playerDeck2Size = matchData.playerDecks.at(player1Id).size();
	packet << playerDeck2Size;
	for (unsigned int i = 0; i < playerDeck2Size; ++i)
	{
		packet << matchData.playerDecks.at(player1Id).at(i);
	}

	
	unsigned int playerHand1Size = matchData.playerHands.at(player0Id).size();
	packet << playerHand1Size;
	for (unsigned int i = 0; i < playerHand1Size; ++i)
	{
		packet << matchData.playerHands.at(player0Id).at(i);
	}
	cof::Debug("[Game Server] playerHand1Size {}", playerHand1Size);
	unsigned int playerHand2Size = matchData.playerHands.at(player1Id).size();
	packet << playerHand2Size;
	for (unsigned int i = 0; i < playerHand2Size; ++i)
	{
		packet << matchData.playerHands.at(player1Id).at(i);
	}
	cof::Debug("[Game Server] playerHand2Size {}", playerHand2Size);
	
	unsigned int playerDiscard1Size = matchData.playerDiscards.at(player0Id).size();
	packet << playerDiscard1Size;
	for (unsigned int i = 0; i < playerDiscard1Size; ++i)
	{
		packet << matchData.playerDiscards.at(player0Id).at(i);
	}

	unsigned int playerDiscard2Size = matchData.playerDiscards.at(player1Id).size();
	packet << playerDiscard2Size;
	for (unsigned int i = 0; i < playerDiscard2Size; ++i)
	{
		packet << matchData.playerDiscards.at(player1Id).at(i);
	}

	
	packet << matchData.heroes.at(player0Id).health;
	packet << matchData.heroes.at(player0Id).maxHealth;
	packet << matchData.heroes.at(player0Id).resource;
	packet << matchData.heroes.at(player0Id).armor;
	packet << matchData.heroes.at(player0Id).attack;
	packet << matchData.heroes.at(player0Id).baseAttack;

	packet << matchData.heroes.at(player1Id).health;
	packet << matchData.heroes.at(player1Id).maxHealth;
	packet << matchData.heroes.at(player1Id).resource;
	packet << matchData.heroes.at(player1Id).armor;
	packet << matchData.heroes.at(player1Id).attack;
	packet << matchData.heroes.at(player1Id).baseAttack;

	packet << (switched ? !matchData.nextStartingPlayer : matchData.nextStartingPlayer);
    packet << player0Id;
	cof::Debug("player0Id: {} and player1Id {} switched {}", player0Id, player0Id, switched);

	SendCustomPacket(ServerGameCommands::SendMatchData, packet, connection);
}

tbsg::Deck* tbsg::GameServer::GetDeckBasedOnString(unsigned int playerID, const ptl::string& deckname)
{
	return this->gameDataDatabase.GetDeck(playerID, deckname);
}

tbsg::Deck* tbsg::GameServer::GetDeckBasedOnID(unsigned int playerID, unsigned int deckId)
{
	return this->gameDataDatabase.GetDeck(playerID, deckId);
}

Packet tbsg::GameServer::CreatePacketForEffectChanges(tbsg::ResultOfRound& matchData)
{
	Packet packet;
	auto sizeOfPlayedCard = static_cast<unsigned int>(matchData.playedCards.size());
	packet << sizeOfPlayedCard;
	for (auto& card : matchData.playedCards)
	{
		packet << card;
	}

	auto sizeOfChanges = static_cast<unsigned int>(matchData.results.size());
	packet << sizeOfChanges;

	for (auto change : matchData.results)
	{
		packet << static_cast<unsigned int>(change.changeType);
		packet << change.change;
		packet << change.index;
	}
	return packet;
}

Packet tbsg::GameServer::CreatePacketForEffectChangesWithSwap(tbsg::ResultOfRound& matchData)
{
	Packet packet;
	const auto sizeOfPlayedCard = static_cast<unsigned int>(matchData.playedCards.size());
	packet << sizeOfPlayedCard;
	for (auto& card : matchData.playedCards)
	{
		packet << card;
	}

	const auto sizeOfChanges = static_cast<unsigned int>(matchData.results.size());
	packet << sizeOfChanges;


	for (auto change : matchData.results)
	{
		
		switch (change.changeType)
		{
		case EffectChange::Hero_Weapon_Attack:
		case EffectChange::Hero_Weapon_Durability:
		case EffectChange::Hero_Health:
		case EffectChange::Hero_Resource:
		case EffectChange::Hero_Armor:
		case EffectChange::Hero_Attack:
		case EffectChange::Hero_Particle:
		case EffectChange::Hero_Animation:
		case EffectChange::Hero_Death:
		case EffectChange::Hero_WinLose:
		case EffectChange::Reward_Attack:
		case EffectChange::Reward_Health:
		case EffectChange::Reward_Armor:
		case EffectChange::Reward_Resource:
		case EffectChange::Reward_Weapon_Attack:
		case EffectChange::Reward_Weapon_Durability:
		case EffectChange::Deck_Shuffle:
		case EffectChange::Deck_DiscardToDeck:
		case EffectChange::Deck_AddCard:
		case EffectChange::Deck_RemoveCard:
		case EffectChange::Deck_DrawCard:
		case EffectChange::Hand_AddCard:
		case EffectChange::Hand_RemoveCard:
		case EffectChange::Discard_AddCard:
		case EffectChange::Discard_RemoveCard:
		case EffectChange::NextStartingPlayer:
		{
			change.index = !change.index;
			break;
		}
		default:
			break;
		}

		packet << static_cast<unsigned int>(change.changeType);
		packet << change.change;
		packet << change.index;
	}
	return packet;
}

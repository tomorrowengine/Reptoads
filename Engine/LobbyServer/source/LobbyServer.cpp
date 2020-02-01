#include "LobbyServer.h"
#include "core/Config.h"
#include "LoggingFunction.h"
#include <utility>
#include "resource_loading/ConfigLoader.h"
#include "net/LobbyCommands.h"
#include "Payloads.h" 

#ifndef _WIN32
#define Sleep usleep
#endif

LobbyServer::LobbyServer(cof::basic_logger::Logger* logger, const std::string& hostname) : Server(logger), hostname(hostname)
{
}

void LobbyServer::Initialize()
{
	this->SetDebug(true);
	cof::Info("{} Initializing LobbyServer", prefix);
	tbsg::LoadConfig("./config.json");

	const tbsg::Config& config = tbsg::Config::Get();
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

	this->StartServer(config.port, config.maxConnection);
}

void LobbyServer::Run()
{
	auto lastTitleUpdate = std::chrono::high_resolution_clock::now();
	auto last = std::chrono::system_clock::now();
	while(this->shouldRun)
	{
		this->ReceivePackets(); 
		this->HandlePackets(); 

		auto nowTitleUpdate = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(nowTitleUpdate - lastTitleUpdate).count() > 5)
		{
			this->UpdateConsoleTitle();
			lastTitleUpdate = nowTitleUpdate;
		}

		auto now = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed = now - last;
		const double untilNextRun = 20 - elapsed.count();
		if(untilNextRun > 0)
		{
			Sleep(untilNextRun);
		}
		last = now;
	}
}

net::IdentifyResponse LobbyServer::IdentifyClient(Packet& packet, net::Connection* connection)
{
	ptl::string token{};
	packet >> token;
	const auto response = this->profileDatabase.CheckSessionToken(connection, token);
	if(response == tbsg::SessionTokenResponse::Success)
	{
		const auto profile = this->profileDatabase.GetProfile(connection);
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

void LobbyServer::HandleCustomPacket(unsigned customCommand, Packet& packet, net::Connection* connection)
{
	auto command = tbsg::ClientLobbyCommands(customCommand);
	if(IsDebug())
	{
		cof::Debug("{} Handling custom packet: ClientLobbyCommands::{} ({})", prefix, GetName(command), customCommand);
	}
	switch (command)
	{

	case tbsg::ClientLobbyCommands::ListLobbies:
	{
		Packet listPacket{};

		listPacket << static_cast<unsigned int>(GetLobbies().size());
		if(IsDebug())
		{
			cof::Debug("{} Number of lobbies: {}", prefix, static_cast<int>(GetLobbies().size()));
		}

		for(const auto& lobby : GetLobbies())
		{
			if(lobby.started)
			{
				continue;
			}

			unsigned int id = lobby.GetLobbyId();

			net::Connection* owner = lobby.GetOwner();
			unsigned int ownerId = (owner == nullptr ? 0 : owner->GetConnectionId());
			ptl::string ownerName = GetPlayerName(owner);

			net::Connection* opponent = lobby.GetOpponent();
			unsigned int opponentId = (opponent == nullptr ? 0 : opponent->GetConnectionId());
			ptl::string opponentName = GetPlayerName(opponent);

			bool started = false; 

			listPacket << id;
			listPacket << ownerId;
			listPacket << ownerName;
			listPacket << opponentId;
			listPacket << opponentName;
			listPacket << started;
		}

		SendCustomPacket(tbsg::ServerLobbyCommands::LobbiesListed, listPacket, connection);
	}
	break;

	case tbsg::ClientLobbyCommands::CreateLobby:
	{
		auto existingLobby = GetLobby(connection);
		if(existingLobby)
		{
			SendCustomPacket(tbsg::ServerLobbyCommands::LobbyCreationFailed, connection);
			break;
		}

		Lobby& lobby = this->lobbies.emplace_back(connection);
		unsigned int id = lobby.GetLobbyId();

		Packet lobbyPacket{};
		lobbyPacket << id;
		SendCustomPacket(tbsg::ServerLobbyCommands::LobbyCreated, packet, connection);
	}
	break;

		case tbsg::ClientLobbyCommands::JoinLobby:
		{
			auto existingLobby = GetLobby(connection);
			if (existingLobby)
			{
				Packet joinFailedPacket{};
				joinFailedPacket << static_cast<unsigned int>(tbsg::LobbyError::AlreadyJoined);
				SendCustomPacket(tbsg::ServerLobbyCommands::JoinLobbyFailed, joinFailedPacket, connection);
				if (IsDebug())
				{
					cof::Error("{} Client tried to join a lobby but already is in lobby {}.", prefix, static_cast<int>(existingLobby->GetLobbyId()));
				}
				break;
			}

			unsigned int lobbyId;
			packet >> lobbyId;
			if(IsDebug())
			{
				cof::Debug("{} Client trying to JoinLobby {}.", prefix, static_cast<int>(lobbyId));
			}

			Lobby* lobby = this->GetLobby(lobbyId);

			if(lobby == nullptr)
			{
				Packet joinFailedPacket {};
				joinFailedPacket << static_cast<unsigned int>(tbsg::LobbyError::Exist);
				SendCustomPacket(tbsg::ServerLobbyCommands::JoinLobbyFailed, joinFailedPacket, connection);
				if (IsDebug())
				{
					cof::Error("{} Client tried to join a lobby that doesn't exist.", prefix, static_cast<int>(lobbyId));
				}
			}
			else
			{
				if(lobby->GetOpponent() != nullptr)
				{
					Packet joinFailedPacket {};
					joinFailedPacket << static_cast<unsigned int>(tbsg::LobbyError::Full);
					SendCustomPacket(tbsg::ServerLobbyCommands::JoinLobbyFailed, joinFailedPacket, connection);
					if (IsDebug())
					{
						cof::Error("{} Client tried to join a lobby that is full.", prefix, static_cast<int>(lobbyId));
					}
				}
				else
				{
					lobby->SetOpponent(connection);

					Packet joinSuccessPacket {};
					joinSuccessPacket << lobby->GetOwner()->GetConnectionId();
					joinSuccessPacket << lobby->GetLobbyId();
					SendCustomPacket(tbsg::ServerLobbyCommands::JoinLobbySuccess, joinSuccessPacket, connection);

					Packet playerJoinedPacket {};
					playerJoinedPacket << connection->GetConnectionId();
					SendCustomPacket(tbsg::ServerLobbyCommands::LobbyPlayerJoined, playerJoinedPacket, lobby->GetOwner());

					if (IsDebug())
					{
						cof::Debug("{} Client joined lobby.", prefix, static_cast<int>(lobbyId));
					}
				}
			}
		}
		break;

	case tbsg::ClientLobbyCommands::LeaveLobby:
	{
		this->HandleLobbyLeave(connection);
		SendCustomPacket(tbsg::ServerLobbyCommands::LobbyLeft, connection);
	}
	break;

	case tbsg::ClientLobbyCommands::SelectDeck:
	{
		cof::Info("DeckSelect");
		auto lobby = this->GetLobby(connection);
		if (!lobby)
		{
			this->SendCustomPacket(tbsg::ServerLobbyCommands::SelectingDeckFailed, connection);
			break;
		}
		unsigned int deckId;
		packet >> deckId;

		auto profile = profileDatabase.GetProfile(connection);
		auto decks = gameDataDatabase.GetDecks(profile->id);
		auto deckIterator = std::find_if(decks.begin(), decks.end(), [&deckId](const tbsg::Deck& deck)
		{
			return deck.id == deckId;
		});
		if (deckIterator == decks.end())
		{
			this->SendCustomPacket(tbsg::ServerLobbyCommands::SelectingDeckFailed, connection);
			break;
		}

		net::Connection* other;
		if(connection->GetConnectionId() == lobby->GetOwner()->GetConnectionId())
		{
			lobby->ownerSelectedDeckId = deckId;
			other = lobby->GetOpponent();
		}
		else if(connection->GetConnectionId() == lobby->GetOpponent()->GetConnectionId())
		{
			lobby->opponentSelectedDeckId = deckId;
			other = lobby->GetOwner();
		}
		else
		{
			this->SendCustomPacket(tbsg::ServerLobbyCommands::SelectingDeckFailed, connection);
			break;
		}

		this->SendCustomPacket(tbsg::ServerLobbyCommands::SelectedDeck, connection);
		if(other != nullptr)
		{
			Packet deckPacket{};
			deckPacket << deckId;
			this->SendCustomPacket(tbsg::ServerLobbyCommands::OpponentSelectedDeck, deckPacket, other);
		}
	}
	break;

	case tbsg::ClientLobbyCommands::LobbyGameStart:
	{
		Packet packet{};
		auto lobby = this->GetLobby(connection);
		if (!lobby)
		{
			cof::Warn("{} [LobbyGameStart] failed because lobby is nullptr", prefix);
			packet << static_cast<unsigned>(tbsg::LobbyError::Unknown);
			this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed,packet, connection);
			break;
		}
		if (!lobby->GetOwner() || !lobby->GetOpponent()) {
			cof::Warn("{} [LobbyGameStart] failed because either owner or opponenet are null", prefix);
			packet << static_cast<unsigned>(tbsg::LobbyError::Unknown);
			this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed,packet, connection);
			break;
		}

		if(lobby->GetOwner()->GetConnectionId() != connection->GetConnectionId())
		{
			cof::Warn("{} [LobbyGameStart] failed because only the owner can start the lobby", prefix);
			packet << static_cast<unsigned>(tbsg::LobbyError::OnlyTheOwnerCanStartTheGame);
			this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed, packet, connection);
			break;
		}

		auto server = this->GetAvailableGameServer();
		if(server.id == 0)
		{
			cof::Warn("{} [LobbyGameStart] failed no game server NoGameServerAvailable", prefix);
			packet << static_cast<unsigned>(tbsg::LobbyError::NoGameServerAvailable);
			this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed,packet, lobby->GetOwner());
			this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed, packet, lobby->GetOpponent());
			break;
		}

		tbsg::Profile* ownerProfile = this->profileDatabase.GetProfile(lobby->GetOwner());
		tbsg::Profile* opponentProfile = this->profileDatabase.GetProfile(lobby->GetOpponent());
		if (!ownerProfile || !opponentProfile)
		{
			cof::Warn("{} [LobbyGameStart] failed !ownerProfile || !opponentProfile", prefix);
			packet << static_cast<unsigned>(tbsg::LobbyError::NoGameServerAvailable);
			this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed,packet, lobby->GetOwner());
			this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed,packet, lobby->GetOpponent());
			break;
		}

		tbsg::Deck* ownerDeck = nullptr;
		tbsg::Deck* opponentDeck = nullptr;
		auto& ownerDecks = this->gameDataDatabase.GetDecks(ownerProfile->id);
		auto& opponentDecks = this->gameDataDatabase.GetDecks(opponentProfile->id);
		if (lobby->ownerSelectedDeckId == 0 && lobby->opponentSelectedDeckId == 0)
		{
			if (ownerDecks.empty() || opponentDecks.empty())
			{
				cof::Warn("{} [LobbyGameStart] failed ownerDecks.empty() || opponentDecks.empty()", prefix);
				packet << static_cast<unsigned>(tbsg::LobbyError::NoDeckSelected);
				this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed,packet, lobby->GetOwner());
				this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed, packet, lobby->GetOpponent());
				break;
			}
			ownerDeck = &(ownerDecks[0]);
			opponentDeck = &(opponentDecks[0]);
		}
		else
		{
			if (lobby->ownerSelectedDeckId == 0 || lobby->opponentSelectedDeckId == 0)
			{
				cof::Warn("{} [LobbyGameStart] failed lobby->ownerSelectedDeckId == {} || lobby->opponentSelectedDeckId == {}", prefix, lobby->ownerSelectedDeckId, lobby->opponentSelectedDeckId);
				packet << static_cast<unsigned>(tbsg::LobbyError::NoDeckSelected);
				this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed,packet, lobby->GetOwner());
				this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed,packet, lobby->GetOpponent());
				break;
			}

			auto ownerDeckIterator = std::find_if(ownerDecks.begin(), ownerDecks.end(), [&lobby](tbsg::Deck& deck)
			{
				return deck.id == lobby->ownerSelectedDeckId;
			});
			auto opponentDeckIterator = std::find_if(opponentDecks.begin(), opponentDecks.end(), [&lobby](tbsg::Deck& deck)
			{
				return deck.id == lobby->opponentSelectedDeckId;
			});
			if (ownerDeckIterator == ownerDecks.end() || opponentDeckIterator == opponentDecks.end())
			{
				cof::Warn("{} [LobbyGameStart] ownerDeckIterator == ownerDecks.end() || opponentDeckIterator == opponentDecks.end()", prefix);
				packet << static_cast<unsigned>(tbsg::LobbyError::NoDeckSelected);
				this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed,packet, lobby->GetOwner());
				this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed,packet, lobby->GetOpponent());
				break;
			}
			ownerDeck = &*ownerDeckIterator;
			opponentDeck = &*opponentDeckIterator;
		}
		
		tbsg::Match match{};
		match.ownerProfileId = ownerProfile->id;
		match.opponentProfileId = opponentProfile->id;
		match.ownerDeckId = ownerDeck->id;
		match.opponentDeckId = opponentDeck->id;
		match.serverId = server.id;

		match = this->api.RegisterMatch(match);

		lobby->started = true;

		Packet serverPacket{};
		serverPacket << server.ip;
		serverPacket << server.port;
		this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStarted, serverPacket, lobby->GetOwner());
		cof::Info("Send LobbyGameStarted LobbyGameStart Owner");
		this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStarted, serverPacket, lobby->GetOpponent());
		cof::Info("Send LobbyGameStarted LobbyGameStart Oppenent");
	}
	break;

	case tbsg::ClientLobbyCommands::AIGameStart:
	{
		auto server = this->GetAvailableGameServer();
		if (server.id == 0)
		{
			this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed, connection);
			break;
		}

		tbsg::Profile* profile = this->profileDatabase.GetProfile(connection);
		if (!profile)
		{
			this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed, connection);
			break;
		}

		auto& decks = this->gameDataDatabase.GetDecks(profile->id);
		if (decks.empty())
		{
			this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStartFailed, connection);
			break;
		}
		auto deck = &(decks[0]);

		tbsg::Match match{};
		match.ownerProfileId = profile->id;
		match.ownerDeckId = deck->id;
		match.opponentProfileId = 0;
		match.opponentDeckId = 0;
		match.serverId = server.id;

		match = this->api.RegisterMatch(match);

		Packet serverPacket{};
		serverPacket << server.ip;
		serverPacket << server.port;
		this->SendCustomPacket(tbsg::ServerLobbyCommands::LobbyGameStarted, serverPacket, connection);
		cof::Info("Send LobbyGameStarted AIGameStart");
	}
	break;

	case tbsg::ClientLobbyCommands::LobbyGameStop:
	{
		
	}
	break;

	case tbsg::ClientLobbyCommands::GetProfileName:
	{
		unsigned int profileId{};
		packet >> profileId;

		auto profile = profileDatabase.GetProfile(profileId);
		if(!profile)
		{
			if(IsDebug())
			{
				cof::Warn("{} A profile was requested that doesn't exist.", prefix);
			}
			assert(false && "Requested profile not found!");
			break;
		}

		Packet namePacket{};
		namePacket << profile->id;
		namePacket << profile->username;
		SendCustomPacket(tbsg::ServerLobbyCommands::ProfileName, namePacket, connection);
	}
	break;

	}
}

void LobbyServer::OnPlayerDisconnected(net::Connection* connection)
{
	const auto profile = this->profileDatabase.GetProfile(connection);
	if(profile)
	{
		cof::Info("{} Client {} ({}) disconnected", prefix, profile->username, connection->GetConnectionId());
		profile->connectionId = 0;
	}
	else
	{
		cof::Info("{} Client {} disconnected", prefix, connection->GetConnectionId());
	}

	this->HandleLobbyLeave(connection);
}

void LobbyServer::OnPlayerConnected(net::Connection* connection)
{
}

void LobbyServer::OnPlayerIdentified(net::Connection* connection)
{
	tbsg::Profile* profile = this->profileDatabase.GetProfile(connection);
	this->gameDataDatabase.LoadDecksOfProfile(profile->id);

	this->api.RegisterIp(profile->id, NetUtils::EnetAddressToIpString(connection->GetPeer()->address));
}

std::string LobbyServer::GetPlayerName(net::Connection* connection)
{
	if(connection)
	{
		const auto profile = profileDatabase.GetProfile(connection);
		if (profile)
		{
			return profile->username;
		}
	}
	return std::string{};
}

void LobbyServer::SendCustomPacket(tbsg::ServerLobbyCommands command, net::Connection* connection)
{
	Packet emptyPacket{};
	SendCustomPacket(command, emptyPacket, connection);
}

void LobbyServer::SendCustomPacket(tbsg::ServerLobbyCommands command, Packet& packet, net::Connection* connection)
{
	const auto commandId = static_cast<unsigned int>(command);
	if (IsDebug())
	{
		cof::Debug("{} Sending custom packet: ServerLobbyCommands::{} ({})", prefix, GetName(command), commandId);
	}
	Server::SendCustomPacket(commandId, packet, connection);
}

const std::vector<Lobby>& LobbyServer::GetLobbies() const
{
	return this->lobbies;
}

Lobby* LobbyServer::GetLobby(unsigned lobbyId)
{
	for (Lobby& lobby : this->lobbies)
	{
		if (lobby.GetLobbyId() == lobbyId)
		{
			return &lobby;
		}
	}
	return nullptr;
}

Lobby* LobbyServer::GetLobby(const net::Connection* connection)
{
	for (auto& lobby : this->lobbies)
	{
		if (lobby.GetOwner() != nullptr && lobby.GetOwner()->GetConnectionId() == connection->GetConnectionId())
		{
			return &lobby;
		}
		if(lobby.GetOpponent() != nullptr && lobby.GetOpponent()->GetConnectionId() == connection->GetConnectionId())
		{
			return &lobby;
		}
	}
	return nullptr;
}

void LobbyServer::ListLobbiesInConsole()
{
	if (!IsDebug())
		return;
	cof::Debug("{} Number of lobbies: {}", prefix, static_cast<int>(lobbies.size()));
	for(const auto& lobby : this->GetLobbies())
	{
		cof::Debug("{}", prefix);
		this->ListLobbyInConsole(&lobby);
	}
}

void LobbyServer::ListLobbyInConsole(const Lobby* lobby)
{
	if (!IsDebug())
		return;
	const auto id = lobby->GetLobbyId();

	net::Connection* owner = lobby->GetOwner();
	const auto ownerId = (owner == nullptr ? 0 : owner->GetConnectionId());

	net::Connection* opponent = lobby->GetOpponent();
	const auto opponentId = (opponent == nullptr ? 0 : opponent->GetConnectionId());

	cof::Debug("{} lobbyId: {}", prefix, static_cast<int>(id));
	cof::Debug("{} ownerId: {}", prefix, static_cast<int>(ownerId));
	cof::Debug("{} opponentId: {}", prefix, static_cast<int>(opponentId));
}

void LobbyServer::HandleLobbyLeave(net::Connection* connection)
{
	assert(connection != nullptr && "connection is nullptr");
	auto lobby = GetLobby(connection);
	if (!lobby)
	{
		return;
	}
	if (lobby->GetOwner()->GetConnectionId() == connection->GetConnectionId())
	{
		if(!lobby->started)
		{
			SendCustomPacket(tbsg::ServerLobbyCommands::LobbyPlayerLeft, lobby->GetOwner());
		}

		if (lobby->GetOpponent())
		{
			lobby->SetOwner(lobby->GetOpponent());
			lobby->SetOpponent(nullptr);
			lobby->ownerSelectedDeckId = lobby->opponentSelectedDeckId;
			lobby->opponentSelectedDeckId = 0;

			if(!lobby->started)
			{
				Packet ownershipPacket{};
				ownershipPacket << lobby->GetOwner()->GetConnectionId();
				SendCustomPacket(tbsg::ServerLobbyCommands::LobbyOwnershipTransferred, ownershipPacket, lobby->GetOwner());
			}
		}
		else
		{
			this->RemoveLobby(lobby);
		}
	}
	else if(lobby->GetOpponent()->GetConnectionId() == connection->GetConnectionId())
	{
		lobby->SetOpponent(nullptr);
		lobby->opponentSelectedDeckId = 0;
		if(!lobby->started)
		{
			SendCustomPacket(tbsg::ServerLobbyCommands::LobbyPlayerLeft, lobby->GetOwner());
		}
	}
	else if(!lobby->GetOwner() && !lobby->GetOpponent())
	{
		cof::Warn("{} Lobby was incorrectly cleaned up!", prefix);
		this->RemoveLobby(lobby);
	}
	else
	{
		if(IsDebug())
		{
			cof::Error("{} GetLobby(Connection*) function is broken!", prefix);
		}
		assert(false && "GetLobby(Connection*) function is broken!");
	}
}

bool LobbyServer::RemoveLobby(Lobby* lobby)
{
	const auto lobbyIterator = std::find_if(lobbies.begin(), lobbies.end(), [&lobby](Lobby& searchLobby)
	{
		return searchLobby.GetLobbyId() == lobby->GetLobbyId();
	});
	if (lobbyIterator != lobbies.end())
	{
		if (IsDebug())
		{
			cof::Debug("{} Removing lobby {}.", prefix, static_cast<int>(lobbyIterator->GetLobbyId()));
		}
		lobbies.erase(lobbyIterator);
		return true;
	}
	cof::Error("{} Trying to remove a lobby that doesn't exist in the lobby vector.", prefix);
	assert(false && "Trying to remove a lobby that doesn't exist in the lobby vector.");
	return false;
}


const tbsg::Server LobbyServer::GetAvailableGameServer() const
{
	auto servers = this->api.GetAvailableServers(tbsg::Config::Get().IsProduction());
	if (servers.empty())
	{
		return tbsg::Server{};
	}

	auto server = servers[0];
	auto selectedServer = std::find_if(servers.begin(), servers.end(), [this](tbsg::Server& server)
		{
			return server.hostname == this->hostname;
		});
	if (selectedServer != servers.end())
	{
		server = *selectedServer;
		if (IsDebug())
		{
			cof::Debug("{} Selected a server with the same hostname: {}", prefix, server.hostname);
		}
	}
	return server;
}

void LobbyServer::UpdateConsoleTitle()
{
#ifdef _WIN32
	SetConsoleTitleA(("LobbyServer (" + tbsg::Config::Get().address + ":" + std::to_string(GetPort()) + ") (Connections: " + std::to_string(GetConnections().size()) + ") " + "(Lobbies: " + std::to_string(lobbies.size()) + ")").c_str());
#endif
}

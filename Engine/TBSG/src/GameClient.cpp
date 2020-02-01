#include "GameClient.h"
#include "Net/GameCommands.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "core/Config.h"

GameClient::~GameClient()
{
	for(auto handle : listenerHandles)
	{
		gameEventQueue->removeListener(handle.first, handle.second);
	}
}

void GameClient::Initialize(ptl::shared_ptr<GameEventQueue> eventQueue)
{
	gameEventQueue = eventQueue;
	
	listenerHandles.insert(std::pair<GameEvent, GameEventQueue::Handle>{ GameEvent::ClientCanStart, gameEventQueue->appendListener(GameEvent::ClientCanStart, 
		[this](Packet&& packet) { ClientIsReady(std::move(packet)); })});
	listenerHandles.insert(std::pair<GameEvent, GameEventQueue::Handle>{ GameEvent::OnCardHover, gameEventQueue->appendListener(GameEvent::OnCardHover, 
		[this](Packet&& packet) { OnCardHover(std::move(packet)); })});
	listenerHandles.insert(std::pair<GameEvent, GameEventQueue::Handle>{ GameEvent::OnCardSelected, gameEventQueue->appendListener(GameEvent::OnCardSelected, 
		[this](Packet&& packet) { OnCardSelected(std::move(packet)); })});
	listenerHandles.insert(std::pair<GameEvent, GameEventQueue::Handle>{ GameEvent::OnCardsSubmitted, gameEventQueue->appendListener(GameEvent::OnCardsSubmitted, 
		[this](Packet&& packet) { OnCardsSubmitted(std::move(packet)); })});

	this->SetDebug(true);
}

void GameClient::Update()
{
	this->ReceivePackets();
	this->HandleEvents();
}

void GameClient::OnConnect()
{
	gameEventQueue->enqueue(GameEvent::OnServerConnect, Packet{});

}

void GameClient::OnDisconnect()
{
	Packet packet{};
	gameEventQueue->enqueue(GameEvent::PlayerDisconnected, std::move(packet));
	gameEventQueue->enqueue(GameEvent::OnServerDisconnect, Packet{});
}

void GameClient::OnIdentificationSuccess()
{
	
	
}

void GameClient::OnIdentificationFailure(net::IdentifyResponse response)
{
	if(response == net::IdentifyResponse::AccountInUse)
	{
		std::cout << "[Error][GameClient] The account being used is already being used on the server." << std::endl;
	}
	else
	{
		std::cout << "[Error][GameClient] Identification Failure" << std::endl;
	}
}

void GameClient::GetIdentity(Packet& packet)
{
	packet << this->accessToken;
}

void GameClient::HandleCustomPacket(unsigned customCommand, Packet& packet)
{
	switch (ServerGameCommands(customCommand))
	{
	case ServerGameCommands::SendLobbyData:
		{
			gameEventQueue->enqueue(GameEvent::LobbyDataReceived, std::move(packet));
			
			break;
		}
	case ServerGameCommands::SendMatchData:
		{
			gameEventQueue->enqueue(GameEvent::MatchDataReceived, std::move(packet));
			break;
		}
	case ServerGameCommands::GameHasStarted:
		{
			gameEventQueue->enqueue(GameEvent::GameHasStarted, std::move(packet));
			break;
		}
	case ServerGameCommands::OpponentOnCardHover: 
		{
			gameEventQueue->enqueue(GameEvent::OpponentOnCardHover, std::move(packet));
			break;
		}		
	case ServerGameCommands::OpponentOnCardSelect: 
		{
			gameEventQueue->enqueue(GameEvent::OpponentOnCardSelected, std::move(packet));
			break;
		}
	case ServerGameCommands::OpponentOnCardsSubmit:
		{
			gameEventQueue->enqueue(GameEvent::OpponentOnCardsSubmitted, std::move(packet));
			break;
		}
	case ServerGameCommands::TurnResult:
		{
			gameEventQueue->enqueue(GameEvent::TurnResult, std::move(packet));
			break;
		}
	case ServerGameCommands::GameServerHasAnError:{
		ASSERT_MSG(false, "GAME SERVER HAS AN ISSUE");
			}
		break;
	default: ;
	}
}

void GameClient::ClientIsReady(Packet&& packet)
{
	SendCustomPacket(static_cast<unsigned int>(ClientGameCommands::ClientCanStart), packet);
}

void GameClient::OnCardHover(Packet&& packet)
{
	SendCustomPacket(static_cast<unsigned int>(ClientGameCommands::OnCardHover), packet);
}

void GameClient::OnCardSelected(Packet&& packet)
{
	SendCustomPacket(static_cast<unsigned int>(ClientGameCommands::OnCardSelect), packet);
}

void GameClient::OnCardsSubmitted(Packet&& packet)
{
	Packet packet_{};
	packet_ << 1;
	packet_.Append(packet.GetData(), packet.GetDataSize());
	SendCustomPacket(static_cast<unsigned int>(ClientGameCommands::OnCardsSubmit), packet_);
}

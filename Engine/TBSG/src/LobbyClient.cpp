#include "LobbyClient.h"
#include "net/LobbyCommands.h"
#include "DatabaseAPI.h"
#include "ClientApplication.h"

void LobbyClient::Initialize(ptl::shared_ptr<LobbyEventQueue> lobbyEventQueue, ClientApplication* clientApplication)
{
	this->lobbyEventQueue = lobbyEventQueue;
	this->clientApplication = clientApplication;
	this->SetDebug(true);


	this->lobbyEventQueue->appendListener(LobbyEvent::RequestLobbyList, [this](std::function<void*()>)
		{
			this->RequestLobbyList();
		});
	this->lobbyEventQueue->appendListener(LobbyEvent::DisconnectFromLobby, [this](std::function<void*()>)
	{
		this->SendCustomPacket(static_cast<unsigned>(tbsg::ClientLobbyCommands::LeaveLobby));
	});
	this->lobbyEventQueue->appendListener(LobbyEvent::CreateNewLobby, [this](std::function<void* ()>)
		{
			this->SendCustomPacket(static_cast<unsigned>(tbsg::ClientLobbyCommands::CreateLobby));
		});

	this->lobbyEventQueue->appendListener(LobbyEvent::JoinExistingLobby, [this](std::function<void*()> data)
		{
			if (!this->joinedLobby)
			{
				unsigned int lobbyID = *static_cast<int*>(data());
				if (lobbyID != 0)
				{
					Packet joinPacket{};
					joinPacket << lobbyID;
					this->SendCustomPacket(static_cast<unsigned>(tbsg::ClientLobbyCommands::JoinLobby), joinPacket);
				}
			}
		});

	this->lobbyEventQueue->appendListener(LobbyEvent::SelectDeck, [this](std::function<void* ()>data)
		{
			unsigned int deckID = *static_cast<unsigned int*>(data());
			if (deckID != 0)
			{
				Packet packet{};
				packet << deckID;
				this->SendCustomPacket(static_cast<unsigned>(tbsg::ClientLobbyCommands::SelectDeck), packet);
			}
			
		});

	this->lobbyEventQueue->appendListener(LobbyEvent::RequestToStartGame, [this](std::function<void* ()>)
		{
			this->SendCustomPacket(static_cast<unsigned int>(tbsg::ClientLobbyCommands::LobbyGameStart));
		});
}

void LobbyClient::Update()
{
	this->ReceivePackets();
	this->HandleEvents();
}

void LobbyClient::OnConnect()
{
	lobbyEventQueue->enqueue(LobbyEvent::OnServerConnect, nullptr);
}

void LobbyClient::OnDisconnect()
{
	lobbyEventQueue->enqueue(LobbyEvent::OnServerDisconnect, nullptr);
}

void LobbyClient::OnIdentificationSuccess()
{
	lobbyEventQueue->enqueue(LobbyEvent::ClientConnected, nullptr);
	this->RequestLobbyList();
}

void LobbyClient::OnIdentificationFailure(net::IdentifyResponse response)
{
	if (response == net::IdentifyResponse::AccountInUse)
	{
		std::cout << "[Error][LobbyClient] The account being used is already being used on the server.\n";
		lobbyEventQueue->enqueue(LobbyEvent::AccountInUse, nullptr);
	}
	else
	{
		lobbyEventQueue->enqueue(LobbyEvent::AuthFailure, nullptr);
		std::cout << "[Error][LobbyClient] Identification Failure\n";
	}
}

void LobbyClient::RequestLobbyList()
{
	printf("%s Requesting lobby list.\n", prefix.c_str());
	this->SendCustomPacket(static_cast<unsigned>(tbsg::ClientLobbyCommands::ListLobbies));
}

void LobbyClient::RequestProfileName(unsigned profileId) const
{
	Packet namePacket{};
	namePacket << profileId;
	SendCustomPacket(static_cast<unsigned>(tbsg::ClientLobbyCommands::GetProfileName), namePacket);
}

void LobbyClient::GetIdentity(Packet& packet)
{
	if(this->accessToken.empty())
	{
		assert(false && "Access Token is empty!");
	}
	packet << this->accessToken;
}

void LobbyClient::HandleCustomPacket(unsigned customCommand, Packet& packet)
{
	auto command = tbsg::ServerLobbyCommands(customCommand);

	printf("Handle custom packet %s\n", GetName(command).c_str());

	switch(command)
	{
		case tbsg::ServerLobbyCommands::LobbiesListed:
		{
			unsigned int lobbyCount = 0;
			packet >> lobbyCount;

			std::vector<tbsg::LobbyData> lobbies{};
			ptl::vector<ui::LobbyDisplayInformation> lobbyDisplayInfo{};

			for(int i = 0; i < lobbyCount; i++)
			{
				tbsg::LobbyData lobby{};
				ui::LobbyDisplayInformation lobbyInfo{};
				packet >> lobby.id;
				packet >> lobby.ownerConnectionId;
				packet >> lobby.ownerName;
				packet >> lobby.opponentConnectionId;
				packet >> lobby.opponentName;
				packet >> lobby.inGame;
				
				if (lobby.ownerName != userName)
				{
					lobbyInfo.lobbyID = lobby.id;
					lobbyInfo.ownerName = lobby.ownerName;
					lobbyDisplayInfo.emplace_back(lobbyInfo);
					lobbies.push_back(lobby);
				}
			}


			
			lobbyEventQueue->enqueue(LobbyEvent::ParseLobbyInfo, [lobbyDisplayInfo]() mutable
				{
					return static_cast<void*>(&lobbyDisplayInfo);
				});
			
		}
		break;
		case tbsg::ServerLobbyCommands::LobbyCreated:
		{
			this->joinedLobby = true;
			unsigned int lobbyID = 0;
			packet >> lobbyID;

			lobbyEventQueue->enqueue(LobbyEvent::SuccessfulLobbyCreated, nullptr);
		}
		break;
		case tbsg::ServerLobbyCommands::LobbyDestroyed:
		{
			lobbyEventQueue->enqueue(LobbyEvent::LobbyDestroyed, nullptr);
		}
		break;
		case tbsg::ServerLobbyCommands::LobbyLeft:
		{
			this->joinedLobby = true;
			lobbyEventQueue->enqueue(LobbyEvent::LobbyLeft, nullptr);
		}
		break;
		case tbsg::ServerLobbyCommands::LobbyLeftFailed:
		{
			lobbyEventQueue->enqueue(LobbyEvent::LobbyLeftFailed, nullptr);
		}
		break;
		case tbsg::ServerLobbyCommands::LobbyPlayerLeft:
		{
			lobbyEventQueue->enqueue(LobbyEvent::LobbyPlayerLeft, nullptr);
		}
		break;
		case tbsg::ServerLobbyCommands::LobbyOwnershipTransferred:
		{
			lobbyEventQueue->enqueue(LobbyEvent::LobbyOwnershipTransferred, nullptr);
		}
		break;
		case tbsg::ServerLobbyCommands::JoinLobbyFailed:
		{
			lobbyEventQueue->enqueue(LobbyEvent::JoinLobbyFailed, nullptr);
		}
		break;
		case tbsg::ServerLobbyCommands::JoinLobbySuccess:
		{
			this->joinedLobby = true;
			lobbyEventQueue->enqueue(LobbyEvent::SuccessfulLobbyCreated, nullptr);
			Packet joinSuccessPacket{};
			
			
			lobbyEventQueue->enqueue(LobbyEvent::JoinExistingLobby, nullptr);
		}
		break;
		case tbsg::ServerLobbyCommands::LobbyPlayerJoined:
		{
			
			lobbyEventQueue->enqueue(LobbyEvent::LobbyPlayerJoined, nullptr);

		}
		break;
		case tbsg::ServerLobbyCommands::LobbyGameStarted:
		{
			ptl::string host;
			packet >> host;
			unsigned int port;
			packet >> port;

			printf("%s Going to connect to GameServer at %s:%u\n", prefix.c_str(), host.c_str(), port);

			if (!this->clientApplication)
			{
				assert(false && "clientApplication is nullptr");
				break;
			}
			this->clientApplication->ConnectToGameServer(host, port);
			lobbyEventQueue->enqueue(LobbyEvent::LobbyGameStarted, nullptr);

		}
		break;
		case tbsg::ServerLobbyCommands::LobbyGameStopped:
		{
			lobbyEventQueue->enqueue(LobbyEvent::LobbyGameStopped, nullptr);
		}
		break;

		case tbsg::ServerLobbyCommands::ProfileName:
		{
			unsigned int profileId{};
			ptl::string username{};
			packet >> profileId;
			packet >> username;
		}
		break;

		case tbsg::ServerLobbyCommands::SelectedDeck:
		{
			Packet deckPacket{};
			unsigned int deckId = 0;
			deckPacket >> deckId;
			if (deckId != 0)
			{
				lobbyEventQueue->enqueue(LobbyEvent::SelectDeck, nullptr);
			}
			else
			{
				lobbyEventQueue->enqueue(LobbyEvent::JoinLobbyFailed, nullptr);
			}
			break;
		}

		case tbsg::ServerLobbyCommands::OpponentSelectedDeck:
		{
			lobbyEventQueue->enqueue(LobbyEvent::OpponentSelectDeck, nullptr);
			break;
		}

		case tbsg::ServerLobbyCommands::LobbyGameStartFailed:
		{
			lobbyEventQueue->enqueue(LobbyEvent::LobbyError, [packet]()mutable
			{
				return static_cast<void*>(&packet);
			});
			
			break;
		}
		default:
		{

		}
		break;
	}
}

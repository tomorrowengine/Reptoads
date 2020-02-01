#include "Net/Server.h"
#include "Net/NetCommands.h"
#include "Net/Packet.h"
#include "Net/NetUtils.h"
#include "Logger.h"

#include <cstdio>

net::Server::Server() : address{}, server(nullptr), debug(false), logger(nullptr)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
	}
	this->connections.reserve(99);
}

net::Server::Server(cof::basic_logger::Logger* logger) : address{}, server(nullptr), debug(false), logger(logger)
{
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
    }
	this->connections.reserve(99);
}

net::Server::~Server()
{
	if (server != nullptr)
	{
		enet_host_destroy(server);
	}

	enet_deinitialize();

	delete logger;
}

void net::Server::StartServer(const unsigned short port, const unsigned short maxSessions)
{
	address.host = ENET_HOST_ANY;
	address.port = port;

	server = enet_host_create(&address, maxSessions * 2, 2, 0, 0);

	if (logger != nullptr)
	{
		if (server == nullptr)
		{
			logger->Error("{} An error occurred while trying to create an ENet server host.", netPrefix);
		}
		logger->Info("{} Successfully started server at {}", netPrefix, address.port);
	}
}

void net::Server::ReceivePackets()
{
	if (server == nullptr)
	{
		return;
	}

	ENetEvent event;
	while (enet_host_service(server, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_NONE:
			break;
		case ENET_EVENT_TYPE_CONNECT:
		{
			if (logger != nullptr)
			{
				logger->Info("{} A new client connected from {}.", netPrefix, NetUtils::EnetAddressToString(event.peer->address));
			}

			unsigned int connectionId = event.data;

			if (connectionId == CONNECTION_ID_INVALID || connectionId > Connection::IDCount())
			{
				connectionId = Connection::NewConnectionId();
			}

			connections.emplace_back(event.peer, connectionId);
			Connection* connection = GetConnection(connectionId);
			OnPlayerConnected(connection);

#ifdef DISABLE_ENCRYPTION
			SendPacket(NetCommands::HandshakeSuccess, event.peer);
#else

			auto rsa = net::NetRSA();

			clientKeys[NetUtils::EnetAddressToString(event.peer->address)] = net::KeyChain{
				rsa,
				{ {} }
			};

			Packet packet;

			auto key = rsa.GetPublicKey();

			auto modulusBytes = static_cast<unsigned int>(key.modBitSize >> 3);
			auto exponentBytes = static_cast<unsigned int>(key.bitSize >> 3);

			packet << modulusBytes;

			for (unsigned int i = 0; i < modulusBytes; i++)
			{
				packet << key.modulus.get()[i];
			}

			packet << exponentBytes;

			for (unsigned int i = 0; i < exponentBytes; i++)
			{
				packet << key.exponent.get()[i];
			}

			SendPacket(NetCommands::HandshakeServerKey, packet, event.peer);
#endif
		}
		break;

		case ENET_EVENT_TYPE_RECEIVE:
		{
			Packet packet;

			packet.OnReceive(event.packet->data, event.packet->dataLength);

			packetQueue.push(std::pair<Connection*, Packet>(GetConnection(event.peer), packet));

			enet_packet_destroy(event.packet);
		}
		break;

		case ENET_EVENT_TYPE_DISCONNECT:
		{
			if (logger)
			{
				logger->Info("{} {} disconnected.", netPrefix, NetUtils::EnetAddressToString(event.peer->address).c_str());
			}

			event.peer->data = nullptr;

			auto connection = std::find_if(connections.begin(), connections.end(), [&event](const net::Connection& connection)
			{
				return connection.GetPeer()->address.port == event.peer->address.port && connection.GetPeer()->address.host == event.peer->address.host;
			});

			if(connection != connections.end())
			{
				this->OnPlayerDisconnected(&*connection);
				connections.erase(connection);
			}
		}
		break;
		}
	}
}

void net::Server::HandlePackets()
{
	while(!packetQueue.empty())
	{
		auto pair = packetQueue.front();

		this->HandleAnyPacket(pair.first, pair.second);

		packetQueue.pop();
	}
}

void net::Server::HandleAnyPacket(Connection* connection, Packet& packet)
{
	unsigned int commandInt;
	packet >> commandInt;
	auto command = NetCommands(commandInt);

	if (command == NetCommands::CustomCommand)
	{
		if (!connection->identified)
		{
			if (debug)
			{
				logger->Warn("{} Client > Server: received custom from {} but connection not identified!", netPrefix, NetUtils::EnetAddressToString(connection->peer->address));
			}
			SendPacket(NetCommands::NotIdentified, connection->GetPeer());
		}
		else
		{
			unsigned int customCommand;
			packet >> customCommand;
			if (debug)
			{
				logger->Debug("{} Client > Server: handling custom {} from {}", netPrefix, static_cast<int>(customCommand), NetUtils::EnetAddressToString(connection->peer->address));
			}
			HandleCustomPacket(customCommand, packet, connection);
		}
	}
	else
	{
		if (debug)
		{
			logger->Debug("{} Client > Server: handling NetCommands {} from {}", netPrefix, GetName(command).c_str(), NetUtils::EnetAddressToString(connection->peer->address));
		}
		HandlePacket(command, packet, connection);
	}
}

const std::vector<net::Connection>& net::Server::GetConnections() const noexcept
{
	return this->connections;
}

net::Connection* net::Server::GetConnection(const unsigned int connectionId)
{
	for (auto& connection : connections)
	{
		if (connection.GetConnectionId() == connectionId)
		{
			return &connection;
		}
	}

	return nullptr;
}

net::Connection* net::Server::GetConnection(ENetPeer* client)
{
	for (auto& connection : connections)
	{
		if (connection.GetPeer()->connectID == client->connectID)
		{
			return &connection;
		}
	}

	return nullptr;
}

void net::Server::SendCustomPacket(unsigned command, Connection* connection)
{
	Packet emptyPacket{};
	SendCustomPacket(command, emptyPacket, connection);
}

void net::Server::SendCustomPacket(unsigned command, Packet& packet, Connection* connection)
{
	if (debug)
		logger->Debug("{} Client < Server: sending custom {} to {}", netPrefix, static_cast<int>(command), NetUtils::EnetAddressToString(connection->peer->address));
	Packet commandPacket{};
	commandPacket << command;
	commandPacket.Append(packet.GetData(), packet.GetDataSize());
	SendPacket(NetCommands::CustomCommand, commandPacket, connection->GetPeer());
}

unsigned net::Server::GetPort() const
{
	return this->address.port;
}

void net::Server::SendPacket(NetCommands command, ENetPeer* client) const
{
	Packet emptyPacket{};
	SendPacket(command, emptyPacket, client);
}

void net::Server::SendPacket(NetCommands command, Packet& packet, ENetPeer* client) const
{
	if (server != nullptr)
	{
		if (debug && command != NetCommands::CustomCommand)
			logger->Debug("{} Client < Server: sending NetCommands {} to {}", netPrefix, GetName(command), NetUtils::EnetAddressToString(client->address));

		Packet commandPacket{};
		commandPacket << static_cast<unsigned int>(command);

		commandPacket.Append(packet.GetData(), packet.GetDataSize());

		size_t size = commandPacket.GetDataSize();
		void* data = new unsigned char[size];
		memcpy(data, commandPacket.GetData(), size);

#ifndef DISABLE_ENCRYPTION
		auto it = clientKeys.find(NetUtils::EnetAddressToString(client->address));

		if (it != clientKeys.end() && it->second.dataKey.GetKey().bitSize > 0)
		{
			size_t encryptedSize;
			std::unique_ptr<unsigned char[]> encryptedData;
			std::unique_ptr<unsigned char[]> iv;

			auto key = it->second.dataKey;
			size_t paddingSize;
			key.Encrypt(static_cast<const unsigned char*>(data), size, iv, encryptedData, encryptedSize, paddingSize);

			Packet cryptoPacket;
			cryptoPacket << static_cast<unsigned int>(NetCommands::CryptoPacket);
			cryptoPacket << static_cast<unsigned int>(paddingSize);
			cryptoPacket.Append(iv.get(), net::NetAES::ivLength >> 3);
			cryptoPacket << static_cast<unsigned int>(encryptedSize);
			cryptoPacket.Append(encryptedData.get(), encryptedSize);

			delete[] static_cast<unsigned char*>(data);

			size = cryptoPacket.GetDataSize();
			data = new unsigned char[size];
			memcpy(data, cryptoPacket.GetData(), size);
		}
#endif
		ENetPacket* epacket = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);

		enet_peer_send(client, 0, epacket);

		delete[] static_cast<unsigned char*>(data);
	}
}

void net::Server::HandlePacket(NetCommands command, Packet& packet, Connection* connection)
{
	switch(command)
	{
		
	case NetCommands::HandshakeDataKey:
	{
		unsigned int size;
		packet >> size;

		auto* encryptedData = new unsigned char[size];

		for (unsigned int i = 0; i < size; i++)
		{
			unsigned char byte;
			packet >> byte;
			encryptedData[i] = byte;
		}

		auto keyChain = clientKeys.find(NetUtils::EnetAddressToString(connection->GetPeer()->address));

		std::unique_ptr<unsigned char[]> decryptedData;
		size_t decryptedDataSize;
		keyChain->second.handshakeKey.Decrypt(encryptedData, size, decryptedData, decryptedDataSize);

		Packet decrypted;
		decrypted.Append(decryptedData.get(), decryptedDataSize);
		unsigned int keySize;

		decrypted >> keySize;

		unsigned char* keyData = new unsigned char[keySize];

		for (unsigned int i = 0; i < keySize; i++)
		{
			unsigned char byte;
			decrypted >> byte;
			keyData[i] = byte;
		}

 		keyChain->second.dataKey = net::NetAES{ {keySize << 3, std::shared_ptr<unsigned char>{keyData, [](unsigned char *p) { delete[] p; } }} };

		delete[] encryptedData;

		SendPacket(NetCommands::HandshakeSuccess, connection->GetPeer());

	} break;
	case NetCommands::CryptoPacket:
	{
		auto it = clientKeys.find(NetUtils::EnetAddressToString(connection->GetPeer()->address));

		if (it != clientKeys.end())
		{
			unsigned int paddingSize;
			packet >> paddingSize;

			unsigned int ivSize = net::NetAES::ivLength >> 3;

			unsigned char* iv = new unsigned char[ivSize];

			for (unsigned int i = 0; i < ivSize; i++)
			{
				unsigned char byte;
				packet >> byte;
				iv[i] = byte;
			}

			unsigned int encryptedSize;
			packet >> encryptedSize;

			unsigned char* encryptedData = new unsigned char[encryptedSize];

			for (unsigned int i = 0; i < encryptedSize; i++)
			{
				unsigned char byte;
				packet >> byte;
				encryptedData[i] = byte;
			}

			auto key = it->second.dataKey;

			std::unique_ptr<unsigned char[]> decryptedData;
			size_t decryptedSize;
			key.Decrypt(encryptedData, encryptedSize, iv, decryptedData, decryptedSize, paddingSize);

			delete[] iv;
			delete[] encryptedData;

			Packet decryptedPacket;
			decryptedPacket.Append(decryptedData.get(), decryptedSize);

			unsigned int commandInt;
			decryptedPacket >> commandInt;

			Packet trimmedPacket;
			trimmedPacket << commandInt;
			trimmedPacket.Append(decryptedPacket.GetData(sizeof(unsigned int)), decryptedPacket.GetDataSize() - sizeof(unsigned int));

			HandleAnyPacket(connection, trimmedPacket);
		}
	} break;
	case NetCommands::Identify:
	{
		auto response = this->IdentifyClient(packet, connection);
		if (response == IdentifyResponse::Success)
		{
			connection->identified = true;
			this->SendPacket(NetCommands::IdentifySuccessful, connection->peer);
			OnPlayerIdentified(connection);
		}
		else
		{
			Packet failurePacket{};
			failurePacket << static_cast<unsigned int>(response);
			this->SendPacket(NetCommands::IdentifyFailure, failurePacket, connection->peer);
		}
	}
	break;

	default:
	{
		cof::Error("{} Received NetCommands {} that isn't implemented.", netPrefix, static_cast<int>(command));
	}
	break;

	}
}

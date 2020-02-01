#include "Net/Client.h"
#include "Net/NetCommands.h"
#include "Net/Packet.h"

#include <cstdio>
#include <numeric>
#include "Crypto/NetAES.h"

net::Client::Client() : client(nullptr), serverPeer(nullptr), keyChain(net::EmptyKeyChain())
{
	queueThread = std::thread(&Client::ReceivePackets, this);
	
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
	}
	std::lock_guard<std::mutex> lock(clientMutex);

	client = enet_host_create(nullptr, 1, 2, 0, 0);
	if (client == nullptr)
	{
		fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
	}
}

net::Client::~Client()
{
	Disconnect();

	alive = false;

	queueThread.join();

	if (serverPeer != nullptr)
	{
		enet_peer_reset(serverPeer);
	}

	enet_deinitialize();

	std::lock_guard<std::mutex> lock(clientMutex);

	client = nullptr;
}

void net::Client::Connect(const char * ip, unsigned short port, unsigned connectionId)
{
	std::lock_guard<std::mutex> lock(clientMutex);

	if (client == nullptr)
	{
		return;
	}

	ENetAddress address;

	enet_address_set_host(&address, ip);
	address.port = port;
	serverPeer = enet_host_connect(client, &address, 2, connectionId);
	if (serverPeer == nullptr)
	{
		fprintf(stderr, "No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}
}

void net::Client::Disconnect() const
{
	if (IsConnected()) {
		enet_peer_disconnect(serverPeer, 0);
	}
}

void net::Client::SendPacket(NetCommands command) const
{
	Packet emptyPacket{};
	this->SendPacket(command, emptyPacket);
}

void net::Client::SendPacket(NetCommands command, Packet& packet, bool encrypted) const
{
	if (client != nullptr) {
		if (serverPeer != nullptr)
		{
			if (debug && command != NetCommands::CustomCommand)
			{
				printf("%s sending %s\n", netPrefix.c_str(), GetName(command).c_str());
			}

			Packet commandPacket = Packet{};
			commandPacket << static_cast<unsigned int>(command);

			commandPacket.Append(packet.GetData(), packet.GetDataSize());

			size_t size = commandPacket.GetDataSize();
			void* data = new unsigned char[size];
			memcpy(data, commandPacket.GetData(), size);
#ifndef DISABLE_ENCRYPTION
			if (encrypted) {
				size_t encryptedSize;
				std::unique_ptr<unsigned char[]> encryptedData;
				std::unique_ptr<unsigned char[]> iv;

				auto key = keyChain.dataKey;
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
			ENetPacket * ePacket = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);

			enet_peer_send(serverPeer, 0, ePacket);

			delete[] static_cast<unsigned char*>(data);
		}
	}
}

void net::Client::SendCustomPacket(unsigned command) const
{
	Packet emptyPacket{};
	this->SendCustomPacket(command, emptyPacket);
}

void net::Client::SendCustomPacket(unsigned command, Packet& packet) const
{
	if (IsConnected()) {
		if (debug)
		{
			printf("%s sending custom %u\n", netPrefix.c_str(), command);
		}

		Packet commandPacket = Packet{};
		commandPacket << static_cast<unsigned int>(command);
		commandPacket.Append(packet.GetData(), packet.GetDataSize());

		SendPacket(NetCommands::CustomCommand, commandPacket);
	}
}

void net::Client::ReceivePackets()
{
	if (client == nullptr)
	{
		return;
	}

	ENetEvent event;
	while (enet_host_service(client, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_NONE:
			break;
		case ENET_EVENT_TYPE_CONNECT: {
			printf("Successfully connected to: %s:%u.\n", NetUtils::EnetHostToIpString(event.peer->address.host).c_str(), event.peer->address.port);

			netEventQueue.push({ event.type, Packet {} });
		}	break;
		case ENET_EVENT_TYPE_RECEIVE: {
			Packet packet;

			packet.OnReceive(event.packet->data, event.packet->dataLength);

			netEventQueue.push({ event.type, packet });

			enet_packet_destroy(event.packet);

		}	break;
		case ENET_EVENT_TYPE_DISCONNECT: {
			printf("Successfully disconnected.\n");
			netEventQueue.push({ event.type, Packet {} });
		} break;
		}
	}
}

void net::Client::HandleEvents()
{
	
	if(client == nullptr)
	{
		return;
	}

	std::lock_guard<std::mutex> lock(queueMutex);

	while(!netEventQueue.empty())
	{
		NetEvent netEvent = netEventQueue.front();

		switch(netEvent.type)
		{
			case ENET_EVENT_TYPE_NONE:
				break;
			case ENET_EVENT_TYPE_CONNECT:
			{
				isConnected = true;
				this->OnConnect();
			}
			break;
			case ENET_EVENT_TYPE_RECEIVE:
			{
				this->HandleAnyPacket(netEvent.packet);
			}
			break;
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				isConnected = false;
				this->OnDisconnect();
			}
			break;
		}

		netEventQueue.pop();
	}
}

void net::Client::HandleAnyPacket(Packet& packet)
{
	unsigned int commandInt;
	packet >> commandInt;
	const auto command = NetCommands(commandInt);

	if (command == NetCommands::CustomCommand)
	{
		unsigned int customCommand;
		packet >> customCommand;
		if (debug)
		{
			printf("%s handling custom %u\n", netPrefix.c_str(), customCommand);
		}
		HandleCustomPacket(customCommand, packet);
	}
	else
	{
		if (debug)
		{
			printf("%s handling NetCommands %s (%u)\n", netPrefix.c_str(), GetName(command).c_str(), command);
		}
		HandlePacket(command, packet);
	}
}

bool net::Client::IsConnected() const
{
	return isConnected;
}

void net::Client::AdditionalPing()
{
	if(serverPeer)
	{
		enet_peer_ping(serverPeer);
		enet_host_flush(client);
	}
}

void net::Client::HandlePacket(NetCommands command, Packet& packet)
{
	switch(command)
	{
	case NetCommands::HandshakeServerKey:
	{
		unsigned int modByteSize;
		packet >> modByteSize;

		auto* modulus = new unsigned char[modByteSize];

		for (unsigned int i = 0; i < modByteSize; i++) {
			unsigned char byte;
			packet >> byte;
			modulus[i] = byte;
		}

		unsigned int expByteSize;
		packet >> expByteSize;

		auto* exponent = new unsigned char[expByteSize];

		for (unsigned int i = 0; i < expByteSize; i++) {
			unsigned char byte;
			packet >> byte;
			exponent[i] = byte;
		}

		net::NetRSAKey serverPublic{
			expByteSize << 3,
			modByteSize << 3,
			0,
			std::shared_ptr<unsigned char>{ modulus, [](unsigned char *p) { delete[] p; } },
			std::shared_ptr<unsigned char>{ exponent, [](unsigned char *p) { delete[] p; } },
			std::shared_ptr<unsigned char>{},
			std::shared_ptr<unsigned char>{},
			std::shared_ptr<unsigned char>{}
		};

		keyChain.handshakeKey = net::NetRSA{ {}, serverPublic };
		keyChain.dataKey = net::NetAES();

		Packet plainResponse;
		plainResponse << static_cast<unsigned int>(keyChain.dataKey.GetKey().bitSize >> 3);
		plainResponse.Append(keyChain.dataKey.GetKey().key.get(), keyChain.dataKey.GetKey().bitSize >> 3);

		std::unique_ptr<unsigned char[]> encryptedData;
		size_t encryptedDataSize;

		keyChain.handshakeKey.Encrypt(static_cast<const unsigned char*>(plainResponse.GetData()), plainResponse.GetDataSize(), encryptedData, encryptedDataSize);

		Packet response;
		response << static_cast<unsigned int>(encryptedDataSize);
		response.Append(encryptedData.get(), encryptedDataSize);

		SendPacket(NetCommands::HandshakeDataKey, response, false);

	}
	break;

	case NetCommands::CryptoPacket:
	{
		unsigned int paddingSize;
		packet >> paddingSize;

		unsigned int ivSize = net::NetAES::ivLength >> 3;

		auto* iv = new unsigned char[ivSize];

		for (unsigned int i = 0; i < ivSize; i++) {
			unsigned char byte;
			packet >> byte;
			iv[i] = byte;
		}

		unsigned int encryptedSize;
		packet >> encryptedSize;

		auto* encryptedData = new unsigned char[encryptedSize];

		for (unsigned int i = 0; i < encryptedSize; i++) {
			unsigned char byte;
			packet >> byte;
			encryptedData[i] = byte;
		}

		auto key = keyChain.dataKey;

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

		HandleAnyPacket(trimmedPacket);
	}
	break;

	case NetCommands::HandshakeSuccess:
	{
		printf("%s Handshake successful\n", netPrefix.c_str());
		auto identificationPacket = Packet{};
		GetIdentity(identificationPacket);
		this->SendPacket(NetCommands::Identify, identificationPacket);
		printf("%s Identifying...\n", netPrefix.c_str());
	}
	break;

	case NetCommands::IdentifySuccessful:
	{
		this->isIdentified = true;
		printf("%s Identify successful\n", netPrefix.c_str());
		this->OnIdentificationSuccess();
	}
	break;

	case NetCommands::IdentifyFailure:
	{
		this->isIdentified = false;
		printf("%s Identify failure!\n", netPrefix.c_str());
		printf("%s The server didn't accept your access token! It might be already in use or it has expired. Please retry logging in.\n", netPrefix.c_str());
		unsigned int response{};
		packet >> response;
		this->OnIdentificationFailure(static_cast<net::IdentifyResponse>(response));
	}
	break;

	default:
	{
		printf("%s Received NetCommands that isn't implemented.", netPrefix.c_str());
	}
	break;

	}
}

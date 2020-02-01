#pragma once

#include "Net/NetUtils.h"
#include "Net/Connection.h"
#include "Net/Packet.h"
#include "NetCommands.h"

#include "Utility/Observable.h"
#include "Crypto/KeyChain.h"
#include "Logger.h"

#include <enet/enet.h>

#include <vector>
#include <map>
#include <queue>
#include <unordered_map>
#include "IdentifyResponse.h"

namespace cof
{
    namespace basic_logger
    {
        class Logger;
    }
}

namespace net
{
	class Server
	{
	public:
		Server();
		explicit Server(cof::basic_logger::Logger* logger);
		virtual ~Server();

		void StartServer(unsigned short port, unsigned short maxSessions);
		void ReceivePackets();
		void HandlePackets();
		void HandleAnyPacket(Connection* connection, Packet& packet);

		
		void SetDebug(bool enableDebug) { this->debug = enableDebug; }
		bool IsDebug() const { return this->debug; }

		const std::vector<Connection>& GetConnections() const noexcept;
		Connection* GetConnection(unsigned int connectionId);
		Connection* GetConnection(ENetPeer* client);

		void SendCustomPacket(unsigned int command, Connection* connection);
		void SendCustomPacket(unsigned int command, Packet& packet, Connection* connection);

		unsigned int GetPort() const;

	private:
		void SendPacket(NetCommands command, ENetPeer* client) const;
		void SendPacket(NetCommands command, Packet& packet, ENetPeer* client) const;

		
		virtual net::IdentifyResponse IdentifyClient(Packet& packet, Connection* connection) = 0;
		
		virtual void OnPlayerConnected(Connection* connection) = 0;
		
		virtual void OnPlayerIdentified(Connection* connection) = 0;

		
		void HandlePacket(NetCommands command, Packet& packet, Connection* connection);
		
		virtual void HandleCustomPacket(unsigned int customCommand, Packet& packet, Connection* connection) = 0;

		virtual void OnPlayerDisconnected(Connection* connection) = 0;

		virtual std::string GetPlayerName(Connection* connection)
		{
			return std::string{};
		}

		
		std::vector<Connection> connections{};

		ENetAddress address;
		ENetHost* server;

		bool debug{ false };

		cof::basic_logger::Logger* logger{ nullptr };
		std::map<std::string, net::KeyChain> clientKeys;
		std::queue<std::pair<Connection*, Packet>> packetQueue{};

		std::string netPrefix = "\u001b[35m[Net Core]\u001b[0m";
	};
}

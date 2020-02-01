#pragma once

#include "Net/NetUtils.h"
#include "Net/Connection.h"
#include "Net/NetCommands.h"
#include "Net/Packet.h"
#include <memory/String.h>
#include "Utility/Observable.h"
#include "Crypto/KeyChain.h"

#include <map>
#include <functional>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include "enet/enet.h"
#include "IdentifyResponse.h"

struct NetEvent {
	ENetEventType type;
	Packet packet;
};

namespace net
{
	class Client
	{
	public:
		Client();
		virtual ~Client();

		void Connect(const char* ip, unsigned short port, unsigned int connectionId = CONNECTION_ID_INVALID);
		void Disconnect() const;
		void SendPacket(NetCommands command) const;
		void SendPacket(NetCommands command, Packet& packet, bool encrypted = true) const;
		
		void SendCustomPacket(unsigned int command) const;
		
		void SendCustomPacket(unsigned int command, Packet& packet) const;

		void ReceivePackets();
		void HandleEvents();
		void HandleAnyPacket(Packet& packet);

		virtual void OnConnect() {};
		virtual void OnDisconnect() {};
		virtual void OnIdentificationSuccess() {};
		virtual void OnIdentificationFailure(net::IdentifyResponse) {};

		bool IsConnected() const;

		
		void SetDebug(bool enableDebug) { this->debug = enableDebug; }

		void AdditionalPing();

	private:
		
		virtual void GetIdentity(Packet& packet) = 0;
		void HandlePacket(NetCommands command, Packet& packet);
		virtual void HandleCustomPacket(unsigned int customCommand, Packet& packet) = 0;

		bool isConnected{ false };
		bool isIdentified{ false };

		ENetHost* client;
		ENetPeer* serverPeer;

		Connection connection;

		std::queue<NetEvent> netEventQueue;

		mutable std::mutex queueMutex;
		mutable std::mutex clientMutex;

		std::thread queueThread;

		std::atomic<bool> alive{ true };
		net::KeyChain keyChain;

		bool debug{ false };

		std::string netPrefix = "[Net Core]";
	};
}

using NetClient = net::Client;

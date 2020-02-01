#pragma once

#include "Net/NetUtils.h"
#include "enet/enet.h"

#define CONNECTION_ID_INVALID 0


namespace net
{
	class Server;

	class Connection
	{
		friend Server;
	public:
		Connection() = default;
		Connection(ENetPeer* peer, unsigned int connectionId);
		~Connection() = default;

		ENetPeer* GetPeer() const noexcept { return peer; }
		unsigned int GetConnectionId() const noexcept { return connectionId; }

		static unsigned int NewConnectionId();
		static unsigned int IDCount() noexcept { return idCount; };

		bool Valid() const;

		bool operator==(const Connection& rhs) const
		{
			return GetConnectionId() == rhs.GetConnectionId();
		}

	private:
		ENetPeer* peer{ nullptr };
		unsigned int connectionId{ CONNECTION_ID_INVALID };
		bool identified{ false };

		static unsigned int idCount;
	};
}

inline bool operator==(const net::Connection& lhs, const net::Connection& rhs) {
	return lhs.GetConnectionId() == rhs.GetConnectionId();
}

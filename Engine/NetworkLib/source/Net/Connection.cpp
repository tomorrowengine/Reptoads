#include "Net/Connection.h"

unsigned int net::Connection::idCount = 0;

net::Connection::Connection(ENetPeer* peer, unsigned int connectionId)
{
	this->peer = peer;
	this->connectionId = connectionId;
}

unsigned net::Connection::NewConnectionId()
{
	idCount++;
	return idCount;
}

bool net::Connection::Valid() const
{
	return this->connectionId != CONNECTION_ID_INVALID;
}

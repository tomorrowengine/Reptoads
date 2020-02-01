#include "Lobby.h"

unsigned int Lobby::lobbyIdCounter = 0;

Lobby::Lobby(net::Connection* owner) : owner(owner)
{
	lobbyIdCounter++;
	lobbyId = lobbyIdCounter;
}

Lobby::Lobby(net::Connection* owner, unsigned lobbyId) : owner(owner), lobbyId(lobbyId) {}

void Lobby::SetOwner(net::Connection* owner)
{
	this->owner = owner;
}

void Lobby::SetOpponent(net::Connection* opponent)
{
	this->opponent = opponent;
}

net::Connection* Lobby::GetOwner() const
{
	return owner;
}

net::Connection* Lobby::GetOpponent() const
{
	return opponent;
}

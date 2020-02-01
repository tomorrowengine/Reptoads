#pragma once
#include <vector>
#include "Net/Connection.h"

class Lobby
{
public:
	explicit Lobby(net::Connection* owner);
	Lobby(net::Connection* owner, unsigned int lobbyId);

	Lobby(const Lobby& a_Other) = default;
	Lobby(Lobby&& a_Other) noexcept = default;
	Lobby& operator=(const Lobby& a_Other) = default;
	Lobby& operator=(Lobby&& a_Other) noexcept = default;

	~Lobby() = default;

	[[nodiscard]] unsigned int GetLobbyId() const noexcept { return lobbyId; }

	void SetOwner(net::Connection* owner);
	void SetOpponent(net::Connection* opponent);

	[[nodiscard]] net::Connection* GetOwner() const;
	[[nodiscard]] net::Connection* GetOpponent() const;

	bool started{ false };

	unsigned int ownerSelectedDeckId{ 0 };
	unsigned int opponentSelectedDeckId{ 0 };

private:
	unsigned int lobbyId;
	static unsigned int lobbyIdCounter;

	net::Connection* owner{ nullptr };
	net::Connection* opponent{ nullptr };

};

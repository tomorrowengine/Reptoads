#pragma once

#include "Net/Server.h"
#include <mariadb++/account.hpp>
#include <mariadb++/connection.hpp>
#include "Lobby.h"
#include "net/LobbyCommands.h"
#include "DatabaseAPI.h"
#include "GameDataDatabase.h"
#include "ProfileDatabase.h"

class LobbyServer : public net::Server
{
public:
	explicit LobbyServer(cof::basic_logger::Logger* logger, const std::string& hostname);
	~LobbyServer() = default;

	void Initialize();
	void Run();

	net::IdentifyResponse IdentifyClient(Packet& packet, net::Connection* connection) override;
	void HandleCustomPacket(unsigned customCommand, Packet& packet, net::Connection* connection) override;
	void OnPlayerDisconnected(net::Connection* connection) override;
	void OnPlayerConnected(net::Connection* connection) override;
	void OnPlayerIdentified(net::Connection* connection) override;
	std::string GetPlayerName(net::Connection* connection) override;

	
	inline void SendCustomPacket(tbsg::ServerLobbyCommands command, net::Connection* connection);
	
	inline void SendCustomPacket(tbsg::ServerLobbyCommands command, Packet& packet, net::Connection* connection);

	const std::vector<Lobby>& GetLobbies() const;
	Lobby* GetLobby(unsigned int lobbyId);
	Lobby* GetLobby(const net::Connection* connection);

	const tbsg::Server GetAvailableGameServer() const;

	void UpdateConsoleTitle();

private:
	void ListLobbiesInConsole();
	void ListLobbyInConsole(const Lobby* lobby);

	
	void HandleLobbyLeave(net::Connection* connection);

	bool RemoveLobby(Lobby* lobby);

	bool shouldRun{ true };

	std::string hostname{};

	db::DatabaseAPI api{};
	tbsg::GameDataDatabase gameDataDatabase{};
	tbsg::ProfileDatabase profileDatabase{};

	std::vector<Lobby> lobbies{};

	std::string prefix = "\u001b[33m[LobbyServer]\u001b[0m";

};

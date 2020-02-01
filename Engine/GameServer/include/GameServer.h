#pragma once
#include "GameDataDatabase.h"
#include "model/GameLogic.h"
#include "gameplay/scripting/LuaContext.h"
#include "core/Config.h"
#include "utils/CMDInput.h"
#include "ProfileDatabase.h"
#include <mariadb++/connection.hpp>
#include "Net/Server.h"
#include "net/GameCommands.h"
#include "eventpp/eventqueue.h"
#include "gameplay/MatchProfileType.h"

class Packet;

namespace tbsg
{
	using ServerEventQueue = eventpp::EventQueue<ClientGameCommands, void(Packet, net::Connection*)>;

	class GameServer : public net::Server
	{
	public:
		friend CMDInput;

		explicit GameServer(cof::basic_logger::Logger* logger, std::string hostname);
		~GameServer();

		void Initialize(int argc, char* argv[]);
		void InitializeQueue();
		
		void Run();

		net::IdentifyResponse IdentifyClient(Packet& packet, net::Connection* connection) override;
		void HandleCustomPacket(unsigned customCommand, Packet& packet, net::Connection* connection) override;
		void OnPlayerDisconnected(net::Connection* connection) override;
		void OnPlayerConnected(net::Connection* connection) override;
		void OnPlayerIdentified(net::Connection* connection) override;
		std::string GetPlayerName(net::Connection* connection) override;

		
		inline void SendCustomPacket(ServerGameCommands command, net::Connection* connection);
		
		inline void SendCustomPacket(ServerGameCommands command, Packet& packet, net::Connection* connection);

		tbsg::MatchProfileType GetMatchProfileType(net::Connection* connection);
		tbsg::MatchProfileType GetMatchProfileType(Profile* profile);

		void UpdateConsoleTitle();

	private:
		void InitializeGameplay();

		db::DatabaseAPI api{};
		scripting::LuaContext luaContext;
		GameDataDatabase gameDataDatabase{};
		ProfileDatabase profileDatabase{};
		GameLogic gameLogic{ gameDataDatabase, profileDatabase };

		tbsg::Match currentMatch{};
		tbsg::MatchData matchData{};

		std::string hostname{};
		unsigned int serverId{};

		enum class LobbyEvent {}; 
		
		std::unique_ptr<ServerEventQueue> serverEQ = std::make_unique<ServerEventQueue>();
		std::unordered_map<ClientGameCommands, ServerEventQueue::Handle> listenerHandles{};

		void RunGameSimulation(ptl::vector<ptl::string>& allArguments);
		void RunOnPlay();

		void CreateGameDataFromDatabase(unsigned int player1Id, unsigned int player2Id, const ptl::string& monsterDeck, const ptl::string& player1Deck, const ptl::string& player2Deck, unsigned int seed = 0);
		void SetupGame(tbsg::Match& matchInformation);
		void SetupPlayerCards();

		void SendOverMatchData(net::Connection* connection, bool switched);

		tbsg::Deck* GetDeckBasedOnString(unsigned int playerID, const ptl::string& deckname);
		tbsg::Deck* GetDeckBasedOnID(unsigned int playerID, unsigned int deckId);

		Packet CreatePacketForEffectChanges(tbsg::ResultOfRound& changes);
		Packet CreatePacketForEffectChangesWithSwap(tbsg::ResultOfRound& changes);

		
		void ClientCanStart(Packet&& packet, net::Connection* connection);
		void CardHover(Packet&& packet, net::Connection* connection);
		void CardSelect(Packet&& packet, net::Connection* connection);
		void CardsSubmit(Packet&& packet, net::Connection* connection);

		const unsigned int GetLobbyOwner();

		net::Connection* DetermineDestinationConnection(net::Connection* connection);

		void TestEventFunc(unsigned int a = 0);

		std::string prefix = "\u001b[33m[GameServer]\u001b[0m";

		net::Connection* firstConnection = nullptr;
		net::Connection* secondConnection = nullptr;

		bool firstSubmitted = false;
		bool secondSubmitted = false;
	};
}

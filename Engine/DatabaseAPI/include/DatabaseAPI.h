#pragma once
#include <string>
#include "DatabaseCredentials.h"
#include <mariadb++/connection.hpp>
#include "Payloads.h"
#include "core/SparseSet.h"

namespace db
{
	class DatabaseAPI
	{
	public:
		DatabaseAPI() = default;
		explicit DatabaseAPI(DatabaseCredentials credentials, unsigned int projectId);
		~DatabaseAPI() = default;

		void OpenDatabaseConnection();

		
		unsigned int GetProfileId(const ptl::string& token) const;
		
		tbsg::Profile GetProfile(unsigned int profileId) const;

		ptl::sparse_set<unsigned int, tbsg::Card> GetCards() const;
		ptl::sparse_set<unsigned int, tbsg::MonsterCard> GetMonsterCards() const;
		ptl::sparse_set<unsigned int, tbsg::CardType> GetCardTypes() const;
		ptl::sparse_set<unsigned int, tbsg::CardRarity> GetCardRarities() const;

		ptl::vector<tbsg::Deck> GetDecksOfProfile(unsigned int profileId, ptl::sparse_set<unsigned int, tbsg::Card>& cards) const;
		
		tbsg::MonsterDeck GetMonsterDeck(unsigned int id, ptl::sparse_set<unsigned int, tbsg::MonsterCard>& cards) const;
		
		tbsg::MonsterDeck GetMonsterDeck(const ptl::string& name, ptl::sparse_set<unsigned int, tbsg::MonsterCard>& cards) const;

		void GetMonsterDecks(ptl::sparse_set<unsigned int, tbsg::MonsterDeck>& monsterDecks,ptl::sparse_set<unsigned int, tbsg::MonsterCard>& cards) const;

		void GetScriptsForCards(ptl::sparse_set<unsigned int, tbsg::Script>& scripts) const;
		
		tbsg::Script GetScriptForCard(unsigned int cardId) const;

		void GetScriptsForMonsterCards(ptl::sparse_set<unsigned int, tbsg::Script>& scripts) const;
		
		tbsg::Script GetScriptForMonsterCard(unsigned int cardId) const;
		
		ptl::vector<tbsg::Script> GetOtherScripts() const;
		
		tbsg::Script GetOtherScript(const ptl::string& scriptName) const;

		
		ptl::vector<tbsg::Server> GetAvailableServers(bool production) const;
		
		unsigned int RegisterServer(const ptl::string& ip, unsigned int port, ptl::string hostname, bool production) const;
		
		void PingServer(unsigned int serverId) const;
		
		void SetServerOccupied(unsigned int serverId, bool isOccupied) const;

		
		void RegisterIp(unsigned int profileId, const ptl::string& ip) const;
		
		void RegisterIp(unsigned int profileId, unsigned int serverId, const ptl::string& ip) const;

		
		const tbsg::Match RegisterMatch(tbsg::Match match) const;
		
		void StartMatch(unsigned int matchId) const;
		
		void EndMatch(unsigned int matchId) const;

		
		const tbsg::Match GetNextMatchForServer(unsigned int serverId) const;

	private:
		unsigned int projectId{};

		DatabaseCredentials credentials{};

		mariadb::account_ref account{};
		mariadb::connection_ref connection{};

		ptl::string databasePrefix = "\u001b[0bm[DatabaseAPI]\u001b[0m";

	};
}

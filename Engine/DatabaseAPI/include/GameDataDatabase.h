#pragma once
#include "Payloads.h"
#include "core/SparseSet.h"
#include "memory/String.h"

#include <memory>
#include <mariadb++/connection.hpp>
#include "DatabaseAPI.h"
class Packet;

namespace tbsg {
	enum class CardDataType : unsigned int
	{
		None = 0, BaseCost = 1, Family = 2, Type = 3, Health = 4, Attack = 5,
	};

	namespace  scripting
	{
		class LuaContext;
	}

	
	class GameDataDatabase
	{
	public:
		GameDataDatabase() = default;

		
		void Initialize(db::DatabaseAPI* api);
		
		void LoadDecksOfProfile(unsigned int profileId);
		
		bool LoadMonsterDeck(unsigned int deckId);
		void LoadMonsterDecks();
		
		bool LoadMonsterDeck(const ptl::string& deckName);

		const ptl::sparse_set<unsigned int, Card>& GetCards() const;
		Card* GetCard(unsigned int id);
        Card* GetCard(std::string name);

		const ptl::sparse_set<unsigned int, MonsterCard>& GetMonsterCards() const;
		MonsterCard* GetMonsterCard(unsigned int id);

		const ptl::sparse_set<unsigned, tbsg::CardType>& GetCardTypes() const;
		const ptl::sparse_set<unsigned, tbsg::CardRarity>& GetCardRarity() const;

		const Script* GetScript(unsigned int id);
		const Script* GetScriptForCard(unsigned int cardId);
		const Script* GetScriptForMonsterCard(unsigned int monsterCardId);
		
		const Script* GetScript(const ptl::string& name);

		const Reward* GetReward(unsigned id);
		const ptl::sparse_set<unsigned, tbsg::Reward>& GetRewards() const;

		
		ptl::vector<Deck>& GetDecks(unsigned int profileId);
		Deck* GetDeck(unsigned int profileId, unsigned int id);
		Deck* GetDeck(unsigned int profileId, const ptl::string& name);

		const ptl::sparse_set<unsigned int, tbsg::MonsterDeck>& GetMonsterDecks();
		tbsg::MonsterDeck* GetMonsterDeck(unsigned int deckId);
		const tbsg::MonsterDeck* GetMonsterDeck(const ptl::string& deckName);
        ptl::vector<unsigned int> GetCardsByType(const ptl::string type);

		
		bool CreateHarddriveCopyOfScript(const ptl::string& requestedString);
	private:
		db::DatabaseAPI* api{};

		ptl::sparse_set<unsigned int, Card> cards{};
		ptl::sparse_set<unsigned int, MonsterCard> monsterCards{};
		ptl::sparse_set<unsigned int, CardType> cardTypes{};
		ptl::sparse_set<unsigned int, CardRarity> cardRarity{};
		ptl::sparse_set<unsigned int, Script> scripts{};
		ptl::sparse_set<unsigned int, Reward> rewards{};
		ptl::unordered_map<unsigned int, ptl::vector<Deck>> decks{};
		ptl::sparse_set<unsigned int, MonsterDeck> monsterDecks{};
	};
}

#pragma once
#include "memory/String.h"
#include "core/SparseSet.h"
#include "Payloads.h"



namespace tbsg
{
	struct Card;
	struct Deck;
	struct MonsterDeck;
}

class GameDataLoader
{
public:
	GameDataLoader() = default;

	ptl::vector<ptl::string> LoadMapSequence(unsigned int mapID);

	bool ParseGameData(const ptl::string& jsonString);
	bool ParseDeckData(const ptl::string& jsonString,  ptl::vector<tbsg::Card>& loadedCardsToCheckWith);

	ptl::vector<tbsg::Card>&& GetLoadedCards() { return std::move(cards); }
	ptl::vector<tbsg::CardRarity>&& GetLoadedCardRarities() { return std::move(cardRarities); }
	ptl::vector<tbsg::CardType>&& GetLoadedCardTypes() { return std::move(cardTypes); }
	ptl::vector<tbsg::MonsterCard>&& GetLoadedMonsterCards() { return std::move(monsterCards); }
	ptl::vector<tbsg::MonsterDeck>&& GetLoadedMonsterDecks() { return std::move(monsterDecks); }
	ptl::vector<tbsg::Deck>&& GetLoadedDecks() { return std::move(playerDecks); }
private:
	ptl::vector<tbsg::Card> cards{};
	ptl::vector<tbsg::CardRarity> cardRarities{};
	ptl::vector<tbsg::CardType> cardTypes{};
	ptl::vector<tbsg::MonsterCard> monsterCards{};
	ptl::vector<tbsg::MonsterDeck> monsterDecks{};
	ptl::vector<tbsg::Deck> playerDecks{};

	tbsg::Card* GetMatchCardObject(const unsigned int cardID);
	tbsg::Card* GetMatchCardObject(const unsigned int cardID, ptl::vector<tbsg::Card>& loadedCardsToCheckWith);
	tbsg::MonsterCard* GetMatchMonsterCardObject(const unsigned int cardID);
};


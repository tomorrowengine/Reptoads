#pragma once
#include <Payloads.h>
#include "memory/Containers.h"
#include "core/SparseSet.h"
#include <climits>
#include "DataStructs.h"
#include "memory/smart_ptr.h"

class Packet;

class GameDatabase
{
public:
    GameDatabase() = default;
    GameDatabase(const GameDatabase& other)= default;

    
    void SetNetworkId(unsigned int netID);

	void InitMaps(std::size_t size);

	void AddMaps(Packet&& packet);
	void AddMaps(unsigned int id, ptl::vector<ptl::string>&& maps);
	tbsg::Card* GetCard(unsigned int id);
	const ptl::vector<ptl::string>& GetMap(unsigned int id);

	
	void AddCards(ptl::vector<tbsg::Card>&& passedCards) { cards = std::move(passedCards); };
	void AddCardRarities(ptl::vector<tbsg::CardRarity>&& passedcardRarities) { cardRarities = std::move(passedcardRarities); };
	void AddCardTypes(ptl::vector<tbsg::CardType>&& passedcardTypes) { cardTypes = std::move(passedcardTypes); };
	void AddMonsterCards(ptl::vector<tbsg::MonsterCard>&& passedmonsterCards) { monsterCards = std::move(passedmonsterCards); };
	void AddMonsterDecks(ptl::vector<tbsg::MonsterDeck>&& passedmonsterDecks) { monsterDecks = std::move(passedmonsterDecks); };
	void AddDecks(ptl::vector<tbsg::Deck>&& passedPlayerDecks) { playerDecks = std::move(passedPlayerDecks); }
    
    
    unsigned int GetLastPlacedCard() { return m_LastPlacedCard; }
    void SetLobbySelected(unsigned int id) { m_LobbySelected = id; }
    unsigned int GetLobbySelected() { return m_LobbySelected; }

	ptl::vector<tbsg::Card>& GetCardList() { return cards; }
	const ptl::vector<tbsg::Deck>& GetDeckList() { return playerDecks; }
private:
    
    unsigned int m_NetworkID{ UINT_MAX };
    unsigned int m_LastPlacedCard = 1234567;  
    unsigned int m_LobbySelected = 0;

    tbsg::Profile m_profile{};
    
    ptl::vector<tbsg::Unit*> m_Units{};

	ptl::vector<tbsg::Card> cards{};
	ptl::vector<tbsg::CardRarity> cardRarities{};
	ptl::vector<tbsg::CardType> cardTypes{};
	ptl::vector<tbsg::MonsterCard> monsterCards{};
	ptl::vector<tbsg::MonsterDeck> monsterDecks{};
	ptl::vector<tbsg::Deck> playerDecks{};

	
	ptl::sparse_set<unsigned int, ptl::vector<ptl::string>> m_maps{};
	ptl::sparse_set<unsigned int, tbsg::Card> m_cards{};

    
    unsigned int m_currentChapter{};
    tbsg::Round m_currentRound{};
    tbsg::ResultOfRound m_CurrentResultOfRound{};
};

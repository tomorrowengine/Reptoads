#include "GameDataDatabase.h"
#include "Net/Packet.h"
#ifdef _WIN32
#include <filesystem>
#else
#include <experimental/filesystem>
#endif
#include <fstream>
#include <iostream>
#include "LoggingFunction.h"

void tbsg::GameDataDatabase::Initialize(db::DatabaseAPI* api)
{
	this->api = api;

	cof::Info("[GameDataDatabase] Loading GameData from the database...");

	cof::Debug("[GameDataDatabase] Loading Cards from the database...");
	this->cards = api->GetCards();
	cof::Debug("[GameDataDatabase] Loading MonsterCards from the database...");
	this->monsterCards = api->GetMonsterCards();
	cof::Debug("[GameDataDatabase] Loading MonsterCardsDecks from the database...");
	LoadMonsterDecks();
	cof::Debug("[GameDataDatabase] Loading CardTypes from the database...");
	this->cardTypes = api->GetCardTypes();
	cof::Debug("[GameDataDatabase] Loading CardRarities from the database...");
	this->cardRarity = api->GetCardRarities();

	cof::Debug("[GameDataDatabase] Loading CardScripts from the database...");
	this->scripts.reserve(GetCards().size() + GetMonsterCards().size());
    api->GetScriptsForCards(this->scripts);
	
	cof::Debug("[GameDataDatabase] Loading MonsterCardScripts from the database...");
	api->GetScriptsForMonsterCards(this->scripts);
	
	cof::Debug("[GameDataDatabase] Loading other scripts from the database...");
	for (tbsg::Script& script : api->GetOtherScripts())
	{
		if (script.id != 0)
		{
			this->scripts.insert(script.id, script);
		}
	}

	cof::Info("[GameDataDatabase] Done loading from database.");
}

void tbsg::GameDataDatabase::LoadDecksOfProfile(unsigned int profileId)
{
	this->decks[profileId] = api->GetDecksOfProfile(profileId, this->cards);
}

bool tbsg::GameDataDatabase::LoadMonsterDeck(unsigned deckId)
{
	tbsg::MonsterDeck deck = api->GetMonsterDeck(deckId, this->monsterCards);
	if (deck.id == 0)
	{
		return false;
	}
	this->monsterDecks.insert(deck.id, deck);
	return true;
}

void tbsg::GameDataDatabase::LoadMonsterDecks()
{
	api->GetMonsterDecks(this->monsterDecks, this->monsterCards);
}

bool tbsg::GameDataDatabase::LoadMonsterDeck(const ptl::string& deckName)
{
	tbsg::MonsterDeck deck = api->GetMonsterDeck(deckName, this->monsterCards);
	if (deck.id == 0)
	{
		return false;
	}
	this->monsterDecks.insert(deck.id, deck);
	return true;
}

const ptl::sparse_set<unsigned, tbsg::Card>& tbsg::GameDataDatabase::GetCards() const
{
	return cards;
}

tbsg::Card* tbsg::GameDataDatabase::GetCard(unsigned id)
{
	return &cards.at(id);
}

tbsg::Card* tbsg::GameDataDatabase::GetCard(std::string name)
{
    for (auto& card : cards)
    {
        if (card.meta.name == name)
        {
            return const_cast<tbsg::Card*>(&card);
        }
    }

    return nullptr;
}

const ptl::sparse_set<unsigned, tbsg::MonsterCard>& tbsg::GameDataDatabase::GetMonsterCards() const
{
	return this->monsterCards;
}

tbsg::MonsterCard* tbsg::GameDataDatabase::GetMonsterCard(unsigned id)
{
	return &this->monsterCards.at(id);
}

const tbsg::Script* tbsg::GameDataDatabase::GetScript(unsigned id)
{
	return &scripts.at(id);
}

const tbsg::Script* tbsg::GameDataDatabase::GetScriptForCard(unsigned cardId)
{
	for (const tbsg::Script& script : this->scripts)
	{
		if (script.cardId == cardId)
		{
			return &script;
		}
	}
	return nullptr;
}

const tbsg::Script* tbsg::GameDataDatabase::GetScriptForMonsterCard(unsigned monsterCardId)
{
	for (const tbsg::Script& script : this->scripts)
	{
		if (script.monsterCardId == monsterCardId)
		{
			return &script;
		}
	}
	return nullptr;
}

const tbsg::Script* tbsg::GameDataDatabase::GetScript(const ptl::string& name)
{
	for (const tbsg::Script& script : this->scripts)
	{
		if (script.name == name)
		{
			return &script;
		}
	}
	return nullptr;
}

const tbsg::Reward* tbsg::GameDataDatabase::GetReward(unsigned id)
{
	return &rewards.at(id);
}

const ptl::sparse_set<unsigned, tbsg::Reward>& tbsg::GameDataDatabase::GetRewards() const
{
	return rewards;
}

const ptl::sparse_set<unsigned, tbsg::CardType>& tbsg::GameDataDatabase::GetCardTypes() const
{
	return cardTypes;
}

const ptl::sparse_set<unsigned, tbsg::CardRarity>& tbsg::GameDataDatabase::GetCardRarity() const
{
	return cardRarity;
}

ptl::vector<tbsg::Deck>& tbsg::GameDataDatabase::GetDecks(unsigned profileId)
{
	return decks[profileId];
}

tbsg::Deck* tbsg::GameDataDatabase::GetDeck(unsigned int profileId, unsigned id)
{
	auto& decks = this->decks.at(profileId);
	for (auto& deck : decks)
	{
		if (deck.id == id)
		{
			return &deck;
		}
	}
	return nullptr;
}

tbsg::Deck* tbsg::GameDataDatabase::GetDeck(unsigned int profileId, const ptl::string& name)
{
	auto& decks = this->decks.at(profileId);
	ptl::vector<Deck>::iterator deckIttr = std::find_if(decks.begin(), decks.end(), [&name](Deck& deck)
		{
			return deck.name == name;
		});
	return &*deckIttr;
}

const ptl::sparse_set<unsigned int, tbsg::MonsterDeck>& tbsg::GameDataDatabase::GetMonsterDecks()
{
	return this->monsterDecks;
}

tbsg::MonsterDeck* tbsg::GameDataDatabase::GetMonsterDeck(unsigned deckId)
{
	return &this->monsterDecks.at(deckId);
}

bool tbsg::GameDataDatabase::CreateHarddriveCopyOfScript(const ptl::string& requestedString)
{
	unsigned int scriptID = 0;
	for (const auto& script : scripts)
	{
		if (script.name == requestedString)
		{
			scriptID = script.id;
		}
	}
	if (scriptID != 0)
	{
		const tbsg::Script* scriptToWrite = this->GetScript(scriptID);
		if (scriptToWrite != nullptr)
		{
			const std::string path = std::string(scriptToWrite->name + ".lua");
			std::ofstream fout(path);
			fout << std::string(scriptToWrite->code);
			fout.close();
			return true;
		}
	}
	return false;
}

const tbsg::MonsterDeck* tbsg::GameDataDatabase::GetMonsterDeck(const ptl::string& deckName)
{
	for (const tbsg::MonsterDeck& deck : monsterDecks)
	{
		if (deck.name == deckName)
		{
			return &deck;
		}
	}
	return nullptr;
}

ptl::vector<unsigned int> tbsg::GameDataDatabase::GetCardsByType(const ptl::string type)
{
	ptl::vector<unsigned int> cardList{};
    int typeID = 0;
    for (auto cardType : cardTypes)
    {
        if (cardType.name == type)
        {
            typeID = cardType.id;
            break;
        }   
    }

    for (auto& card : cards)
    {
        if (card.meta.type == typeID)
        {
            cardList.emplace_back(card.id);
        }
    }
    return std::move(cardList);
}

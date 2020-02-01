#include <sstream>
#include "utils/GameDataLoader.h"
#include "rapidjson/rapidjson.h"
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include "core/StrHash.h"
#include "Payloads.h"
#include <iostream>

ptl::vector<ptl::string> GameDataLoader::LoadMapSequence(unsigned int mapID)
{
    ptl::string filepath = "./data/maps.json";
    ptl::vector<ptl::string> mapNames;

    rapidjson::GenericDocument< rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator < rapidjson::CrtAllocator > > mapSeqDoc;
    char readBuffer[512];
    FILE* fp;
    errno_t err = fopen_s(&fp, filepath.c_str(), "r");
    if (err)
    {
        std::cout << "[Warn] GameDataLoader::LoadMapSequence Could not load map file " << filepath << " error: " << err << std::endl;
    }
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    mapSeqDoc.ParseStream(is);
    if (mapSeqDoc.HasParseError())
    {
        std::cout << "[Warn]GameDataLoader::LoadMapSequence Syntax Error file " << filepath << std::endl;
        fclose(fp);
    } else
    {
        auto sequences = mapSeqDoc["sequences"].GetArray();
        for (auto& seq : sequences)
        {
            ptl::string name = seq["name"].GetString();
            unsigned int id = tbsg::HashString(name);
            if (id == mapID)
            {
                auto maps = seq["maps"].GetArray();
                for (auto& map : maps)
                {
                    mapNames.push_back(map.GetString());
                }
            }
        }
        fclose(fp);
        std::cout << "[Info] GameDataLoader: Loaded " << mapNames.size() << " chapter(s) from DB" << std::endl;
    }
    return std::move(mapNames);
}

bool GameDataLoader::ParseGameData(const ptl::string& jsonString)
{
	if (jsonString.empty())
	{
		return true;
	}
	rapidjson::Document jsonResult;
	jsonResult.Parse(jsonString.c_str());

	
	rapidjson::Value& cardArray = jsonResult["cards"];
	cards.reserve(100);
	if (cardArray.IsArray())
	{
		for (rapidjson::Value::ConstValueIterator itr = cardArray.Begin(); itr != cardArray.End(); ++itr)
		{
			const rapidjson::Value& cardAttibute = *itr;
			if (cardAttibute.IsObject())
			{
				tbsg::Card cardObj;
				cardObj.id = cardAttibute["id"].GetInt();
				cardObj.meta.description = cardAttibute["description"].GetString();
				cardObj.meta.name = cardAttibute["name"].GetString();
				cardObj.meta.rarity = cardAttibute["cardRarityId"].GetInt();
				cardObj.meta.type = cardAttibute["cardTypeId"].GetInt();
				cards.push_back(cardObj);
			}
		}
	}
	else
	{
		return false;
	}
	
	rapidjson::Value& cardRarityArray = jsonResult["cardRarity"];
	cardRarities.reserve(10);
	if (cardRarityArray.IsArray())
	{
		for (rapidjson::Value::ConstValueIterator itr = cardRarityArray.Begin(); itr != cardRarityArray.End(); ++itr)
		{
			const rapidjson::Value& cardRarityAttibute = *itr;
			if (cardRarityAttibute.IsObject())
			{
				tbsg::CardRarity rarity;
				rarity.id = cardRarityAttibute["id"].GetInt();
				rarity.name = cardRarityAttibute["name"].GetString();
				cardRarities.push_back(rarity);
			}
		}
	}
	else
	{
		return false;
	}
	
	rapidjson::Value& cardTypesArray = jsonResult["cardTypes"];
	ptl::vector<ptl::string> cardTypeVec{};
	cardTypeVec.reserve(10);
	if (cardTypesArray.IsArray())
	{
		for (rapidjson::Value::ConstValueIterator itr = cardTypesArray.Begin(); itr != cardTypesArray.End(); ++itr)
		{
			const rapidjson::Value& cardTypeAttribute = *itr;
			if (cardTypeAttribute.IsObject())
			{
				tbsg::CardType cardType;
cardType.id = cardTypeAttribute["id"].GetInt();
cardType.name = cardTypeAttribute["name"].GetString();
cardTypes.push_back(cardType);
			}
		}
	}
	else
	{
	return false;
	}

	
	rapidjson::Value& monsterCardArray = jsonResult["monsters"];
	monsterCards.reserve(100);
	if (monsterCardArray.IsArray())
	{
		for (rapidjson::Value::ConstValueIterator itr = monsterCardArray.Begin(); itr != monsterCardArray.End(); ++itr)
		{
			const rapidjson::Value& monsterCardAttibute = *itr;
			if (monsterCardAttibute.IsObject())
			{
				tbsg::MonsterCard monsterCardObj;
				monsterCardObj.id = monsterCardAttibute["id"].GetInt();
				monsterCardObj.meta.name = monsterCardAttibute["name"].GetString();
				monsterCardObj.meta.description = monsterCardAttibute["description"].GetString();
				monsterCardObj.meta.type = monsterCardAttibute["cardTypeId"].GetInt();
				monsterCardObj.data.maxHealth = monsterCardAttibute["maxHealth"].GetInt();
				monsterCardObj.data.health = monsterCardObj.data.maxHealth;
				monsterCardObj.data.monsterTrait = monsterCardAttibute["trait"].GetInt();
				monsterCards.push_back(monsterCardObj);
			}
		}
	}
	else
	{
		return false;
	}

	
	rapidjson::Value& monsterDeckArray = jsonResult["monsterDecks"];
	monsterDecks.reserve(100);
	if (monsterDeckArray.IsArray())
	{
		for (rapidjson::Value::ConstValueIterator itr = monsterDeckArray.Begin(); itr != monsterDeckArray.End(); ++itr)
		{
			const rapidjson::Value& monsterDeckAttibute = *itr;
			if (monsterDeckAttibute.IsObject())
			{
				tbsg::MonsterDeck monsterDeckObj;
				monsterDeckObj.id = monsterDeckAttibute["id"].GetInt();
				monsterDeckObj.name = monsterDeckAttibute["name"].GetString();
				auto& cardPile = monsterDeckObj.cards;
				if (monsterDeckAttibute["cards"].IsArray())
				{
					for (rapidjson::Value::ConstValueIterator itr = monsterDeckAttibute["cards"].Begin(); itr != monsterDeckAttibute["cards"].End(); ++itr)
					{
						const rapidjson::Value& cardIter = *itr;
						auto monsterCardRef = this->GetMatchMonsterCardObject(cardIter.GetInt());
						if (monsterCardRef != nullptr)
						{
							cardPile.push_back(monsterCardRef);
						}
					}
				}
				monsterDecks.push_back(monsterDeckObj);
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool GameDataLoader::ParseDeckData(const ptl::string& jsonString, ptl::vector<tbsg::Card>& loadedCardsToCheckWit)
{
	if (jsonString.empty())
	{
		return true;
	}
	rapidjson::Document jsonResult;
	jsonResult.Parse(jsonString.c_str());
	
	if (jsonResult.HasMember("data"))
	{
		rapidjson::Value& deckArray = jsonResult["data"];

		playerDecks.reserve(25);
		if (deckArray.IsArray())
		{
			for (rapidjson::Value::ConstValueIterator itr = deckArray.Begin(); itr != deckArray.End(); ++itr)
			{
				const rapidjson::Value& deckAttibute = *itr;
				if (deckAttibute.IsObject())
				{
					tbsg::Deck deckObj;
					deckObj.id = deckAttibute["id"].GetInt();
					deckObj.name = deckAttibute["name"].GetString();
					if (deckAttibute.HasMember("cards"))
					{
						auto& cardPile = deckObj.cards;
						cardPile.reserve(35);
						if (deckAttibute["cards"].IsArray())
						{
							for (rapidjson::Value::ConstValueIterator itr = deckAttibute["cards"].Begin(); itr != deckAttibute["cards"].End(); ++itr)
							{
								const rapidjson::Value& cardIter = *itr;
								auto CardObj = this->GetMatchCardObject(cardIter.GetInt(), loadedCardsToCheckWit);
								if (CardObj != nullptr)
								{
									cardPile.push_back(CardObj);
								}
							}
						}
						playerDecks.push_back(deckObj);
					}
				}
			}
			return true;
		}
	}
	return false;
}

tbsg::Card* GameDataLoader::GetMatchCardObject(const unsigned int cardID)
{
	for (auto& card : cards)
	{
		if (card.id == cardID)
		{
			return &card;
		}
	}
	return nullptr;
}

tbsg::Card* GameDataLoader::GetMatchCardObject(const unsigned int cardID,  ptl::vector<tbsg::Card>& loadedCardsToCheckWith)
{
	for (auto& card : loadedCardsToCheckWith)
	{
		if (card.id == cardID)
		{
			return &card;
		}
	}
	return nullptr;
}

tbsg::MonsterCard* GameDataLoader::GetMatchMonsterCardObject(const unsigned int cardID)
{
	for (auto& card : monsterCards)
	{
		if (card.id == cardID)
		{
			return &card;
		}
	}
	return nullptr;
}

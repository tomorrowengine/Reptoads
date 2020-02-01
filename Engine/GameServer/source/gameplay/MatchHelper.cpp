#include <algorithm>
#include <numeric>
#include "gameplay/MatchHelper.h"
#include "GameDataDatabase.h"
#include "gameplay/scripting/LuaContext.h"
#include "scripting/LuaBindingFunc.h"
#include "gameplay/GamePlayHelper.h"
#include "core/StrHash.h"
#include "LoggingFunction.h"
#include <chrono>
#include <random>

tbsg::MatchData tbsg::gameplay::CreateMatchData(GameDataDatabase* gameDataModel, ptl::string monsterDeck, ptl::string playerOneDeck,
	ptl::string playerTwoDeck, unsigned int playerOneID , unsigned int playerTwoID, unsigned int seed)
{
	ptl::vector<MonsterCard> monsterCards;
	auto deck = gameDataModel->GetDeck(0, monsterDeck.c_str());
	tbsg::gameplay::CreateCardListFromDeck(*deck);
	ptl::vector<tbsg::Deck> decks;
	decks.push_back(*gameDataModel->GetDeck(0, playerOneDeck.c_str()));
	decks.push_back(*gameDataModel->GetDeck(0, playerTwoDeck.c_str()));
	return CreateMatchData(monsterCards, decks, ptl::vector<ptl::vector<Card*>>{}, ptl::vector<tbsg::Hero>{}, 2, 1);
}

tbsg::MatchData tbsg::gameplay::CreateMatchData(tbsg::MonsterDeck& monsterDeck, ptl::vector<tbsg::Deck>& decksToUse,
	ptl::vector<ptl::vector<Card*>> cardsInHand, ptl::vector<tbsg::Hero> heroObjsToUse, unsigned int playerOneID, unsigned int playerTwoID,
	unsigned int playerCount, unsigned int playerNumberThatStarts, unsigned int seed)
{
    ptl::vector<tbsg::MonsterCard> monsterCards;

    for (auto& monsterCard : monsterDeck.cards)
    {
        monsterCards.push_back(*monsterCard);
    }
	return CreateMatchData(monsterCards, decksToUse, cardsInHand, heroObjsToUse, playerOneID, playerTwoID, playerCount, playerNumberThatStarts, seed);
}


tbsg::MatchData tbsg::gameplay::CreateMatchData(ptl::vector<tbsg::MonsterCard>& monsterCards, tbsg::Deck& player1Deck, tbsg::Deck& player2Deck, 
	ptl::vector<ptl::vector<Card*>> cardsInHand, ptl::vector<tbsg::Hero> heroObjsToUse, unsigned int playerOneID, unsigned int playerTwoID, 
	unsigned int playerCount, unsigned int playerNumberThatStarts, unsigned int seed)
{
	auto decks = ptl::vector<tbsg::Deck>{ player1Deck, player2Deck };
	return CreateMatchData(monsterCards, decks, cardsInHand, heroObjsToUse, playerOneID, playerTwoID, playerCount,playerNumberThatStarts, seed);
}







tbsg::MatchData tbsg::gameplay::CreateMatchData(
	ptl::vector<tbsg::MonsterCard>& monsterCards, ptl::vector<tbsg::Deck>& decksToUse,
	ptl::vector<ptl::vector<Card*>> cardsInHand, ptl::vector<tbsg::Hero> heroObjsToUse,
	unsigned int playerOneID, unsigned int playerTwoID, unsigned int playerCount, 
	unsigned int playerThatStarts, unsigned int seed)
{
	tbsg::MatchData matchObj;

	
	if (!monsterCards.empty())
	{
		matchObj.monsterCards = monsterCards;
	}

	
	if (playerCount > 2)
	{
		unsigned int addingAmount = playerCount - 2;
		tbsg::gameplay::CreateContainer(matchObj.playerDecks, addingAmount);
		tbsg::gameplay::CreateContainer(matchObj.playerHands, addingAmount);
		tbsg::gameplay::CreateContainer(matchObj.playerDiscards, addingAmount);
		tbsg::gameplay::CreateContainer(matchObj.heroes, addingAmount);
	}

	
	
	for (unsigned int pCount = 0; pCount < playerCount; ++pCount)
	{
		
		
		if (!decksToUse.empty() && (static_cast<unsigned int>(decksToUse.size()) > pCount))
		{
			if (!decksToUse[pCount].cards.empty() && pCount < playerCount)
			{
				matchObj.playerDecks[pCount] = tbsg::gameplay::ConvertDeckToCardNumbers(decksToUse[pCount]);
			}
		}
		
		if (!cardsInHand.empty() && (static_cast<unsigned int>(cardsInHand.size()) > pCount))
		{
			if (!cardsInHand[pCount].empty() && pCount < playerCount)
			{
				matchObj.playerHands[pCount] = tbsg::gameplay::ConvertCardListToCardIDs(cardsInHand[pCount]);
			}
		}

		
		if (!heroObjsToUse.empty() && (static_cast<unsigned int>(heroObjsToUse.size()) > pCount))
		{
			if (heroObjsToUse[pCount].health != 30 || heroObjsToUse[pCount].maxHealth != 30 ||
				heroObjsToUse[pCount].resource != 0 || heroObjsToUse[pCount].armor != 0 ||
				heroObjsToUse[pCount].attack != 1 || heroObjsToUse[pCount].baseAttack != 1)
			{
				matchObj.heroes[pCount] = heroObjsToUse[pCount];
			}
		}
		

		if (seed == 0)
		{
            auto unix_timestamp = std::chrono::seconds(std::time(nullptr));
            int unix_timestamp_x_1000 = std::chrono::milliseconds(unix_timestamp).count();
			unsigned int totalNumber = matchObj.playerDecks.size() + matchObj.monsterCards.size() + matchObj.playerReadyState + matchObj.playerAmount + unix_timestamp_x_1000;
			matchObj.gameSeed = std::mt19937{ totalNumber };
			matchObj.seed = totalNumber;
		}
		else {
			matchObj.gameSeed = std::mt19937{ seed };
			matchObj.seed = seed;
		}
	}

	
	matchObj.nextStartingPlayer = playerThatStarts;
	return matchObj;
}


bool tbsg::gameplay::CreateContainer(ptl::vector<ptl::vector<unsigned int>>& toAddTowards, unsigned int amountToAdd , unsigned int addUntilLimit )
{
	if (amountToAdd == 0)
	{
		return false;
	}
	if ((toAddTowards.size() + amountToAdd) > addUntilLimit && addUntilLimit != 0)
	{
		amountToAdd = addUntilLimit - toAddTowards.size();
	}
	ptl::vector<unsigned int> newElements(amountToAdd, 0);
	toAddTowards.push_back(newElements);
	return true;
}

bool tbsg::gameplay::CreateContainer(ptl::vector<tbsg::Hero>& toAddTowards, unsigned int amountToAdd , unsigned int addUntilLimit )
{
	if (amountToAdd == 0)
	{
		return false;
	}
	if ((toAddTowards.size() + amountToAdd) > addUntilLimit && addUntilLimit != 0)
	{
		amountToAdd = addUntilLimit - toAddTowards.size();
	}
	ptl::vector<tbsg::Hero> newElements{};
	
	for (unsigned int i = 0; i < amountToAdd; ++i)
	{
		newElements.push_back(tbsg::Hero{});
	}
	
	toAddTowards.insert(std::end(toAddTowards), std::begin(newElements), std::end(newElements));
	return true;
}

bool tbsg::gameplay::FillContainer(ptl::vector<unsigned int>& toAddTowards, unsigned int amountToAdd, unsigned int addUntilLimit )
{
	if (amountToAdd == 0)
	{
		return false;
	}
	if ((toAddTowards.size() + amountToAdd) > addUntilLimit && addUntilLimit != 0)
	{
		amountToAdd = addUntilLimit - toAddTowards.size();
	}
	ptl::vector<unsigned int> newElements(amountToAdd, 0);
	toAddTowards.insert(std::end(toAddTowards), std::begin(newElements), std::end(newElements));
	return true;
}

ptl::vector<unsigned int> tbsg::gameplay::ConvertDeckToCardNumbers(const tbsg::Deck& toCreateFrom)
{
	return ConvertCardListToCardIDs(toCreateFrom.cards);
}

ptl::vector<unsigned int> tbsg::gameplay::ConvertCardListToCardIDs(const ptl::vector<tbsg::Card*>& toCreateFrom)
{
	ptl::vector<unsigned int> newList(toCreateFrom.size(), 0);

	for (unsigned int i = 0; i < toCreateFrom.size(); ++i)
	{
		newList[i] = toCreateFrom[i]->id;
	}

	return newList;
}

bool tbsg::gameplay::FillContainer(ptl::vector<tbsg::Deck>& toAddTowards, unsigned int amountToAdd , unsigned int addUntilLimit )
{
	if (amountToAdd == 0)
	{
		return false;
	}
	if ((toAddTowards.size() + amountToAdd) > addUntilLimit)
	{
		amountToAdd = addUntilLimit - toAddTowards.size();
	}
	ptl::vector<tbsg::Deck> newElements(amountToAdd, tbsg::Deck{});
	toAddTowards.insert(std::end(toAddTowards), std::begin(newElements), std::end(newElements));
	return true;
}

bool tbsg::gameplay::FillContainer(ptl::vector<tbsg::Card*>& toAddTowards, unsigned int amountToAdd, unsigned int addUntilLimit)
{
	if (amountToAdd == 0)
	{
		return false;
	}
	if ((toAddTowards.size() + amountToAdd) > addUntilLimit)
	{
		amountToAdd = addUntilLimit - toAddTowards.size();
	}
	ptl::vector<tbsg::Card*> newElements(amountToAdd, new tbsg::Card);
	toAddTowards.insert(std::end(toAddTowards), std::begin(newElements), std::end(newElements));
	return true;
}


ptl::vector<unsigned int> tbsg::gameplay::CreatePlaceHolderList(const unsigned int amountToFill)
{
	return ptl::vector<unsigned int> (amountToFill, 0);
}

ptl::vector<unsigned int> tbsg::gameplay::CreateCardListFrom(const ptl::vector<unsigned int>& createFromExistingIDS)
{
	ptl::vector<unsigned int> newList;
	newList.reserve(createFromExistingIDS.size());
	std::copy(createFromExistingIDS.begin(), createFromExistingIDS.end(), std::back_inserter(newList));

	return newList;
}

ptl::vector<unsigned int> tbsg::gameplay::CreateCardListFrom(const ptl::vector<tbsg::Card*>& createFromExistingCards)
{
	ptl::vector<unsigned int> newList;
	newList.reserve(createFromExistingCards.size());
	for (auto& card : createFromExistingCards)
	{
		newList.push_back(card->id);
	}
	return newList;
}

ptl::vector<unsigned int> tbsg::gameplay::CreateCardListFrom(const tbsg::Deck& createFromExistingDeck)
{
	ptl::vector<unsigned int> newList;
	newList.reserve(createFromExistingDeck.cards.size());
	for (auto& card : createFromExistingDeck.cards)
	{
		newList.push_back(card->id);
	}
	return newList;
}

ptl::vector<tbsg::Card*> tbsg::gameplay::CreateCardListFromDeck(const tbsg::Deck& createFromExistingDeck)
{
	ptl::vector<tbsg::Card*> newList;
	newList.reserve(createFromExistingDeck.cards.size());
	for (auto& card : createFromExistingDeck.cards)
	{
		newList.push_back(card);
	}
	return newList;
}

void tbsg::gameplay::PlaceCardListTo(ptl::vector<unsigned int>& toPlaceIn, const ptl::vector<unsigned int> toCopyFrom)
{
	if (toPlaceIn.empty())
	{
		toPlaceIn = CreateCardListFrom(toCopyFrom);
	}
	else
	{
		toPlaceIn.insert(std::end(toPlaceIn), std::begin(toCopyFrom), std::end(toCopyFrom));
	}
}

void tbsg::gameplay::PlaceCardListTo(ptl::vector<unsigned int>& toPlaceIn, const ptl::vector<tbsg::Card*>& toCopyFrom)
{

	if (toPlaceIn.empty())
	{
		toPlaceIn = CreateCardListFrom(toCopyFrom);
	}
	else
	{
		ptl::vector<unsigned int> list = CreateCardListFrom(toCopyFrom);
		toPlaceIn.insert(std::end(toPlaceIn), std::begin(list), std::end(list));
	}
}


void tbsg::gameplay::PlaceCardListTo(ptl::vector<unsigned int>& toPlaceIn, const tbsg::Deck& toCopyFrom)
{
	if (toPlaceIn.empty())
	{
		toPlaceIn = CreateCardListFrom(toCopyFrom);
	}
	else
	{
		ptl::vector<unsigned int> list = CreateCardListFrom(toCopyFrom);
		toPlaceIn.insert(std::end(toPlaceIn), std::begin(list), std::end(list));
	}
}

using EPlayerIndex = unsigned int;

tbsg::ResultOfRound tbsg::gameplay::OnPlay(tbsg::MatchData & matchData, scripting::LuaContext& luaContext, GameDataDatabase* gameDataModel)
{

	auto TEST = [&matchData](auto index)
	{
		for(auto card : matchData.playerHands[index])
		{
			if( card > 6000)
			{
				return true;
			}
		}
		for (auto card : matchData.playerDecks[index])
		{
			if (card > 6000)
			{
				return true;
			}
		}
		for (auto card : matchData.playerDiscards[index])
		{
			if (card > 6000)
			{
				return true;
			}
		}
		return false;
	};

    ResultOfRound currentResultOfRound{};
	

	auto KillPlayer = [&matchData](unsigned int idx, auto& isHeroDead) {
		if (matchData.heroes[idx].health <= 0) {
			isHeroDead[idx] = true;
		}
	};

	auto AddChange = [&currentResultOfRound](Change&& change) {
		currentResultOfRound.results.emplace_back(std::move(change));
	};

    
    ptl::vector<EPlayerIndex> playersID{matchData.nextStartingPlayer,!matchData.nextStartingPlayer };

    
    matchData.nextStartingPlayer = !matchData.nextStartingPlayer;

	AddChange({ tbsg::EffectChange::NextStartingPlayer, static_cast<int>(matchData.nextStartingPlayer), static_cast<unsigned int>(matchData.nextStartingPlayer) });

	bool isSImulation = false;

    
    


    for (auto currentPlayerId : playersID)
    {
		auto cardId = matchData.playedCards[currentPlayerId];
         if (cardId != 0)
        {
            

			 auto itr =  std::find_if(matchData.playerHands[currentPlayerId].begin(), matchData.playerHands[currentPlayerId].end(),[&cardId](unsigned int id)
			{
				 return id == cardId;
			 });
			 if(itr != matchData.playerHands[currentPlayerId].end()){
				matchData.playerHands[currentPlayerId].erase(itr);
			 }

			 matchData.playerDiscards[currentPlayerId].push_back(cardId);
			 
			 AddChange({ EffectChange::Discard_AddCard, static_cast<int>(cardId), static_cast<unsigned int>(currentPlayerId) });
        }else
        {
			 isSImulation = true;
        }
    }

    int currentPlayer = 0;

    
    ptl::vector<bool> isHeroDead{ false, false };

    bool isAtBossFight = matchData.monsterCards.size() == 1;
    bool isAtEndFight = matchData.monsterCards.size() == 0;


    
    tbsg::scripting::BindMatchFunc(luaContext, &matchData, currentPlayer, matchData.monsterCards, matchData.playedCards, gameDataModel, currentResultOfRound, isHeroDead, isAtBossFight, isAtEndFight);

    bool monsterDead = false;
    

    for (auto currentPlayerId : playersID)
    {
        currentPlayer = currentPlayerId;

        
		auto currentCardId = matchData.playedCards[currentPlayer];
        if (isHeroDead[currentPlayer] == false && currentCardId != 0)
        {

            
            gameplay::BaseCardEffect(matchData, currentPlayer, gameDataModel->GetCard(currentCardId)->data.baseCardEffects, currentResultOfRound,isAtEndFight);

            
            gameplay::CallFunctionFromLua(luaContext, *gameDataModel->GetScriptForCard(currentCardId), "OnCardPlay", gameDataModel->GetCard(currentCardId));

            KillPlayer(currentPlayer, isHeroDead);
            KillPlayer(!currentPlayer, isHeroDead);

            
            

            
        }

        
        if (HandleDeath(isHeroDead, playersID.size(), matchData))
        {
            
            matchData.isMatchDone = true;

            tbsg::Change changeHeroWin{ tbsg::EffectChange::Hero_WinLose, 1, 0 };
            currentResultOfRound.results.push_back(changeHeroWin);

            tbsg::Change changeHeroLose{ tbsg::EffectChange::Hero_WinLose, 1, 1 };
            currentResultOfRound.results.push_back(changeHeroLose);

            cof::Debug("Both Players lose");
            return currentResultOfRound;
        }

        if (isAtEndFight == false && matchData.monsterCards[0].data.health <= 0 && monsterDead == false)
        {
            
            gameplay::CallFunctionFromLua(luaContext, *gameDataModel->GetScriptForMonsterCard(matchData.monsterCards[0].id), "OnCardDeath", gameDataModel->GetMonsterCard(matchData.monsterCards[0].id),&matchData.heroes[currentPlayer], &matchData.heroes[!currentPlayer]);

            GiveReward(matchData.heroes[currentPlayer], matchData.monsterCards[0].data.reward, currentResultOfRound, currentPlayer);

			AddChange({ tbsg::EffectChange::Card_Death, 0, 0 });

            monsterDead = true;

            for (int heroIndex = 0; heroIndex < isHeroDead.size(); ++heroIndex)
            {
                if (isHeroDead[heroIndex] == true)
                {
                    cof::Debug(("Player: " + std::to_string(!heroIndex) + "Wins the game").c_str());
                    matchData.isMatchDone = true;

					AddChange({ tbsg::EffectChange::Hero_WinLose, 1, static_cast<unsigned int>(heroIndex) });

					AddChange({ tbsg::EffectChange::Hero_WinLose, 0, static_cast<unsigned int>(!heroIndex) });

                    return currentResultOfRound;
                }
            }
        }
        else if (isAtEndFight == true)
        {
            for (int heroIndex = 0; heroIndex < isHeroDead.size(); ++heroIndex)
            {
                if (isHeroDead[heroIndex] == true)
                {
                    cof::Debug(("Player: " + std::to_string(!heroIndex) + "Wins the game").c_str());
                    matchData.isMatchDone = true;

                    AddChange({ tbsg::EffectChange::Hero_WinLose, 1, static_cast<unsigned int>(heroIndex) });

                    AddChange({ tbsg::EffectChange::Hero_WinLose, 0, static_cast<unsigned int>(!heroIndex) });

                    return currentResultOfRound;
                }
            }
        }
    }

	
	if (isAtEndFight == false)
	{
		if (monsterDead == false)
		{
			
			gameplay::CallFunctionFromLua(luaContext, *gameDataModel->GetScriptForMonsterCard(matchData.monsterCards[0].id), "OnCardPlay", &matchData.monsterCards[0]);

			KillPlayer(currentPlayer, isHeroDead);
			KillPlayer(!currentPlayer, isHeroDead);

			
			if (HandleDeath(isHeroDead, playersID.size(), matchData))
			{
				
				matchData.isMatchDone = true;
				cof::Debug("Both Players lose");

				AddChange({ tbsg::EffectChange::Hero_WinLose, 1, 0 });


				AddChange({ tbsg::EffectChange::Hero_WinLose, 1, 1 });
				return currentResultOfRound;
			}
			
			matchData.playerFightDuration++;
		}
		else
		{
			matchData.monsterCards.erase(matchData.monsterCards.begin());

			
			matchData.playerFightDuration = 0;

			AddChange({ tbsg::EffectChange::NextMonster, 0, 0 });
		}

	}

		for (auto correctPlayerIndex : playersID)
		{
            if (isHeroDead[correctPlayerIndex] == false)
            {
                if (matchData.playerDecks[correctPlayerIndex].empty())
                {
                    ptl::vector<unsigned int> cardsDrawn = gameplay::DrawCards(matchData.playerDiscards[correctPlayerIndex], matchData.playerDecks[correctPlayerIndex], matchData.playerDiscards[correctPlayerIndex].size());

                    for (auto card : cardsDrawn)
                    {
                        AddChange({ tbsg::EffectChange::Deck_DiscardToDeck, static_cast<int>(card), static_cast<unsigned int>(correctPlayerIndex) });
                    }

                    gameplay::ShuffleCards(matchData.playerDecks[correctPlayerIndex]);
                    AddChange({ tbsg::EffectChange::Deck_Shuffle, 0, static_cast<unsigned int>(correctPlayerIndex) });
                }

                if (matchData.playerHands[correctPlayerIndex].size() < 6)
                {
                    ptl::vector<unsigned int> cardsDrawn = gameplay::DrawCards(matchData.playerDecks[correctPlayerIndex], matchData.playerHands[correctPlayerIndex], 1);

                    for (auto card : cardsDrawn)
                    {
                        AddChange({ tbsg::EffectChange::Deck_DrawCard, static_cast<int>(card), static_cast<unsigned int>(correctPlayerIndex) });
                    }
                }
                else
                {
                    ptl::vector<unsigned int> cardsDrawn = gameplay::DrawCards(matchData.playerDecks[correctPlayerIndex], matchData.playerDiscards[correctPlayerIndex], 1);

                    for (auto card : cardsDrawn)
                    {
                        AddChange({ tbsg::EffectChange::Discard_AddCard, static_cast<int>(card), static_cast<unsigned int>(correctPlayerIndex) });
                    }
                }
            }
		}

    return currentResultOfRound;
}



void tbsg::gameplay::ResetHeroStats(tbsg::Hero & hero)
{
    hero.armor = 0;
    hero.resource = 0;
    hero.weapon = nullptr;
    hero.attack = hero.baseAttack;
    hero.health = hero.maxHealth;
}

bool tbsg::gameplay::HandleDeath(ptl::vector<bool> isHeroDead, int playerCount, MatchData& matchData)
{
    int deathCount = 0;
    for (const auto& heroDead : isHeroDead)
    {
        if (heroDead)
        {
            deathCount++;
        }
    }

    if (deathCount == playerCount)
    {
        return true;
    }
    return false;
}

void tbsg::gameplay::GiveReward(Hero& hero, ptl::vector<Reward*>& reward, tbsg::ResultOfRound& resultOfRound, int playerID)
{
    for (const auto reward : reward)
    {
        tbsg::Change change;
        change.index = playerID;
        change.change = reward->powerup;

        switch (reward->type)
        {
            case CardRewardType::None:
            {
                return;
            }

            case CardRewardType::Armor:
            {
                hero.armor += reward->powerup;
                change.changeType = tbsg::EffectChange::Reward_Armor;
                cof::Info(("Player " + std::to_string(playerID) + " Got " + std::to_string(reward->powerup) + " Armor").c_str());
                break;
            }

            case CardRewardType::Attack:
            {
                hero.attack += reward->powerup;
                change.changeType = tbsg::EffectChange::Reward_Attack;
                cof::Info(("Player " + std::to_string(playerID) + " Got " + std::to_string(reward->powerup) + " Attack").c_str());

                break;
            }

            case CardRewardType::Resource:
            {
                hero.resource += reward->powerup;
                change.changeType = tbsg::EffectChange::Reward_Resource;
                cof::Info(("Player " + std::to_string(playerID) + " Got " + std::to_string(reward->powerup) + " Resource").c_str());
                break;
            }

            case CardRewardType::Weapon:
            {
                if (reward->weapon != nullptr)
                {
                    hero.weapon = reward->weapon;

                    tbsg::Change changeWeaponAttack;
                    changeWeaponAttack.index = playerID;
                    changeWeaponAttack.change = reward->weapon->attack;
                    changeWeaponAttack.changeType = tbsg::EffectChange::Reward_Weapon_Attack;
                    resultOfRound.results.push_back(changeWeaponAttack);

                    change.change = reward->weapon->durability;
                    change.changeType = tbsg::EffectChange::Reward_Weapon_Durability;
                }
                break;
            }

            case CardRewardType::Health:
            {
                hero.health += reward->powerup;
                change.changeType = tbsg::EffectChange::Reward_Health;
                cof::Info(("Player " + std::to_string(playerID) + " Got " + std::to_string(reward->powerup) + " Health").c_str());
                break;
            }
        }
        resultOfRound.results.push_back(change);
    }
}


void tbsg::gameplay::BaseCardEffect(MatchData& matchData, int currentPlayerID, ptl::vector<BaseCardEffects>& baseCardEffects, tbsg::ResultOfRound& resultOfRound, bool isAtEndFight)
{

	auto DiscardFromHand = [&matchData,&resultOfRound](auto& baseCardEffect,auto currentPlayerID)
	{
		for (int i = 0; i < baseCardEffect.effectValue; ++i)
		{
			auto& playerHand = matchData.playerHands[currentPlayerID];
			if (!playerHand.empty())
			{
				int randomIndex = gameplay::GetRandomIntInRange(0, playerHand.size() - 1, matchData.gameSeed);
				auto cardId = playerHand[randomIndex];
				tbsg::Change changeHandRemoveCard;
				changeHandRemoveCard.change = cardId;
				changeHandRemoveCard.changeType = tbsg::EffectChange::Hand_RemoveCard;
				changeHandRemoveCard.index = currentPlayerID;
				resultOfRound.results.push_back(changeHandRemoveCard);

                tbsg::Change changeDiscardAddCard;
                changeDiscardAddCard.change = cardId;
                changeDiscardAddCard.changeType = tbsg::EffectChange::Discard_AddCard;
                changeDiscardAddCard.index = currentPlayerID;
                resultOfRound.results.push_back(changeDiscardAddCard);

				matchData.playerDiscards[currentPlayerID].push_back(cardId);
				cof::Info("Discard {} from player {}`s Hand", cardId, currentPlayerID);
				gameplay::RemoveCard(playerHand, cardId);
			}
		}
	};

	auto DrawFromDeck = [&matchData,&resultOfRound](auto amount,auto currentPlayerID)
	{
        int overflow = 0;
        if(matchData.playerHands[currentPlayerID].size() + amount >= 6)
        {
            overflow = matchData.playerHands[currentPlayerID].size() + amount - 6;
            amount = 6 - matchData.playerHands[currentPlayerID].size();
        }

		ptl::vector<unsigned int> cardsDrawn = gameplay::DrawCards(matchData.playerDecks[currentPlayerID], matchData.playerHands[currentPlayerID], amount);

		for (auto card : cardsDrawn)
		{
			tbsg::Change changeHandAddCard;
			changeHandAddCard.change = card;
			changeHandAddCard.changeType = tbsg::EffectChange::Deck_DrawCard;
			changeHandAddCard.index = currentPlayerID;
			cof::Info("Deck_DrawCard {} from player {}`s Hand", card, currentPlayerID);
			resultOfRound.results.push_back(changeHandAddCard);
		}

        if(overflow > 0)
        {
            ptl::vector<unsigned int> cardsDiscarded = gameplay::DrawCards(matchData.playerDecks[currentPlayerID], matchData.playerDiscards[currentPlayerID], overflow);

            for (auto card : cardsDiscarded)
            {
                tbsg::Change changeHandAddCard;
                changeHandAddCard.change = card;
                changeHandAddCard.changeType = tbsg::EffectChange::Discard_AddCard;
                changeHandAddCard.index = currentPlayerID;
				cof::Info("Discard_AddCard {} from player {}`s Hand", card, currentPlayerID);
                resultOfRound.results.push_back(changeHandAddCard);
            }
        }
	};


    for (auto& baseCardEffect : baseCardEffects)
    {
        switch (baseCardEffect.baseEffect)
        {
            case BaseEffect::MonsterDamage:
            {
                if (isAtEndFight)
                {
                    gameplay::DamageToHero(baseCardEffect.effectValue, matchData.heroes[!currentPlayerID], !currentPlayerID, resultOfRound);
                }
                else
                {
                    gameplay::DamageToCard(baseCardEffect.effectValue, matchData.monsterCards[0], resultOfRound);
                }
                break;
            }
            case BaseEffect::OpponentDamage:
            {
                gameplay::DamageToHero(baseCardEffect.effectValue, matchData.heroes[!currentPlayerID], !currentPlayerID , resultOfRound);
                break;
            }
            case BaseEffect::SelfDamage:
            {
                gameplay::DamageToHero(baseCardEffect.effectValue, matchData.heroes[currentPlayerID], currentPlayerID, resultOfRound);
                break;
            }
            case BaseEffect::MonsterHealth:
            {
                if (isAtEndFight)
                {
                    gameplay::HeroAddHealth(baseCardEffect.effectValue, matchData.heroes[!currentPlayerID], !currentPlayerID, resultOfRound);
                }
                else
                {
                    gameplay::CardAddHealth(baseCardEffect.effectValue, matchData.monsterCards[0], resultOfRound);
                }
                break;
            }
            case BaseEffect::OpponentHealth:
            {
                gameplay::HeroAddHealth(baseCardEffect.effectValue, matchData.heroes[!currentPlayerID], !currentPlayerID, resultOfRound);
                break;
            }
            case BaseEffect::SelfHealth:
            {
                gameplay::HeroAddHealth(baseCardEffect.effectValue, matchData.heroes[currentPlayerID], currentPlayerID, resultOfRound);
                break;
            }
            case BaseEffect::MonsterArmor:
            {
                if (isAtEndFight)
                {
                    gameplay::HeroAddArmor(baseCardEffect.effectValue, matchData.heroes[!currentPlayerID], !currentPlayerID, resultOfRound);
                }
                else
                {
                    gameplay::CardAddArmor(baseCardEffect.effectValue, matchData.monsterCards[0], resultOfRound);
                }
                break;
            }
            case BaseEffect::OpponentArmor:
            {
                gameplay::HeroAddArmor(baseCardEffect.effectValue, matchData.heroes[!currentPlayerID], !currentPlayerID, resultOfRound);
                break;
            }
            case BaseEffect::SelfArmor:
            {
                gameplay::HeroAddArmor(baseCardEffect.effectValue, matchData.heroes[currentPlayerID], currentPlayerID, resultOfRound);
                break;
            }
            case BaseEffect::OpponentDrawCard:
            {
				DrawFromDeck(baseCardEffect.effectValue, !currentPlayerID);
                break;
            }
            case BaseEffect::SelfDrawCard:
            {
				DrawFromDeck(baseCardEffect.effectValue, currentPlayerID);
                break;
            }
            case BaseEffect::OpponentDiscardCard:
            {
				DiscardFromHand(baseCardEffect, !currentPlayerID);
                break;
            }
            case BaseEffect::SelfDiscardCard:
            {
				DiscardFromHand(baseCardEffect, currentPlayerID);
                break;
            }
			default: cof::Warn("BaseEffect default case was called!"); break;
        }
    }
}

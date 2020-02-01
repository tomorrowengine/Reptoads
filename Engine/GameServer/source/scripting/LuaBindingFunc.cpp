#include <random>
#include <utility>
#include "core/StrHash.h"
#include "scripting/LuaBindingFunc.h"
#include "gameplay/DataStructs.h"
#include "gameplay/scripting/LuaContext.h"
#include "GameDataDatabase.h"
#include "gameplay/GamePlayHelper.h"
#include "LoggingFunction.h"

void tbsg::scripting::BindMatchFunc(LuaContext& lua, MatchData* match, int& currentPlayer, ptl::vector<tbsg::MonsterCard>& monsterCards, ptl::vector<unsigned int> playedCards, GameDataDatabase* database, ResultOfRound& currentResultOfRound, ptl::vector<bool>& isHeroDead, bool& isAtBossFight, bool& isAtEndFight)
{
    lua.SetType<MatchData>("Match");
    lua.SetType<ptl::vector<tbsg::Card*>>("Hand");
    lua.SetType<Deck>("Deck");

    
    std::function<MatchData*()> GetMatch = [match]()
    {
        assert(match != nullptr);
        return match;
    };
    lua.SetFunction("GetMatch", GetMatch);

    
    std::function<ptl::vector<unsigned int>*(int)> GetDeck = [match](int index)
    {
        assert(match != nullptr);
        return &(match->playerDecks[index]);
    };
    lua.SetFunction("GetDeck", GetDeck);

    
    std::function<ptl::vector<unsigned int>*(int)> GetHand = [match](int playerID)
    {
        return &(match->playerHands[playerID]);
    };
    lua.SetFunction("GetHand", GetHand);


    
    std::function<tbsg::Hero*()> GetRival = [match, &currentPlayer]()
    {
        if (currentPlayer == 0)
        {
            return &(match->heroes[1]);
        }
        return &(match->heroes[0]);
    };
    lua.SetFunction("GetRival", GetRival);

    
    std::function<tbsg::Hero*()> GetHero = [match, &currentPlayer]()
    {
        return &match->heroes[currentPlayer];
    };
    lua.SetFunction("GetHero", GetHero);


    
    std::function<tbsg::Hero*(int)> GetHeroByID = [match, &currentPlayer](int playerID)
    {
        return &match->heroes[playerID];
    };
    lua.SetFunction("GetHeroByID", GetHeroByID);

    
    std::function<bool()> AreHeroesAtBoss = [&isAtBossFight]()
    {
        return isAtBossFight;
    };
    lua.SetFunction("AreHeroesAtBoss", AreHeroesAtBoss);

    
    std::function<bool()> AreHeroesAtEndFight = [&isAtEndFight]()
    {
        return isAtEndFight;
    };
    lua.SetFunction("AreHeroesAtEndFight", AreHeroesAtEndFight);

    
    std::function<int()> PlayerFightDuration = [match]()
    {
        return match->playerFightDuration;
    };
    lua.SetFunction("PlayerFightDuration", PlayerFightDuration);


    
    std::function<void(int)> SetFirstPlayerFlag = [match](int ID)
    {
        match->nextStartingPlayer = ID;
    };
    lua.SetFunction("SetFirstPlayerFlag", SetFirstPlayerFlag);

    
    std::function<void()> SwitcheFirstPlayerFlag = [match]()
    {
        match->nextStartingPlayer = !match->nextStartingPlayer;
    };
    lua.SetFunction("SwitcheFirstPlayerFlag", SwitcheFirstPlayerFlag);


    


    std::function<int(Hero*)> GetHeroResource = [](Hero* hero) {
        assert(hero != nullptr);
        return (int)hero->resource;
    };
    lua.SetFunction("GetHeroResource", GetHeroResource);

    
    std::function<void(Hero*, int)> AddHeroResource = [match, &currentResultOfRound](Hero* hero, int amount)
    {
        if (hero != nullptr)
        {
            int resource = hero->resource + amount;

            if (resource < 0)
            {
                AddChangeHero(hero, match, currentResultOfRound, hero->resource * -1, tbsg::EffectChange::Hero_Resource);

                hero->resource = 0;
            }
            else
            {
                hero->resource += amount;
                AddChangeHero(hero, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Resource);

            }
        }
    };
    lua.SetFunction("AddHeroResource", AddHeroResource);


    
    std::function<void(Hero*, int)> SetHeroResource = [match, &currentResultOfRound](Hero* hero, int amount)
    {
        if (hero != nullptr)
        {
            if (amount - hero->resource != 0)
            {
                AddChangeHero(hero, match, currentResultOfRound, amount - hero->resource, tbsg::EffectChange::Hero_Resource);
            }

            hero->resource = amount;
        }
    };
    lua.SetFunction("SetHeroResource", SetHeroResource);


    
    std::function<void(Hero*, Hero*, int)> StealHeroResource = [match, &currentResultOfRound](Hero* target, Hero* receiver, int amount)
    {

        if (target != nullptr && receiver != nullptr)
        {
            if (target->resource >= amount)
            {
                AddChangeHero(target, match, currentResultOfRound, amount * -1, tbsg::EffectChange::Hero_Resource);
                AddChangeHero(receiver, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Resource);

                target->resource -= amount;
                receiver->resource += amount;
            }
            else
            {
                AddChangeHero(target, match, currentResultOfRound, target->resource * -1, tbsg::EffectChange::Hero_Resource);
                AddChangeHero(receiver, match, currentResultOfRound, target->resource, tbsg::EffectChange::Hero_Resource);

                receiver->resource += target->resource;
                target->resource = 0;
            }
        }
    };
    lua.SetFunction("StealHeroResource", StealHeroResource);


    

    
    std::function<int(Hero*)> GetHeroHealth = [](Hero* hero)
    {
        assert(hero != nullptr);
        return (int)hero->health;
    };
    lua.SetFunction("GetHeroHealth", GetHeroHealth);


    
    std::function<void(Hero*, int)> AddHeroHealth = [match, &currentResultOfRound, &isHeroDead](Hero* hero, int amount)
    {
        if (hero != nullptr)
        {
            int newHealth = hero->health + amount;

            if (newHealth <= 0)
            {
                AddChangeHero(hero, match, currentResultOfRound, hero->health * -1, tbsg::EffectChange::Hero_Health);
                int heroID = AddChangeHero(hero, match, currentResultOfRound, 0, tbsg::EffectChange::Hero_Death);
                isHeroDead[heroID] = true;
                hero->health = 0;
            }
            else if (newHealth > hero->maxHealth)
            {
                AddChangeHero(hero, match, currentResultOfRound, hero->maxHealth - hero->health, tbsg::EffectChange::Hero_Health);

                hero->health = hero->maxHealth;
            }
            else
            {
                AddChangeHero(hero, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Health);
                hero->health += amount;
            }
        }
    };
    lua.SetFunction("AddHeroHealth", AddHeroHealth);


    
    std::function<void(Hero*, int)> SetHeroHealth = [match, &currentResultOfRound, &isHeroDead](Hero* hero, int amount)
    {
        if (hero != nullptr)
        {
            AddChangeHero(hero, match, currentResultOfRound, amount - hero->health, tbsg::EffectChange::Hero_Health);

            hero->health = amount;
            if (hero->health <= 0)
            {
                int heroID = AddChangeHero(hero, match, currentResultOfRound, 0, tbsg::EffectChange::Hero_Death);
                isHeroDead[heroID] = true;
                hero->health = 0;
            }

        }
    };
    lua.SetFunction("SetHeroHealth", SetHeroHealth);


    
    std::function<void(Hero*, Hero*, int)> StealHeroHealth = [match, &currentResultOfRound, &isHeroDead](Hero* target, Hero* receiver, int amount)
    {

        if (target != nullptr && receiver != nullptr)
        {
            if (target->health > amount)
            {
                AddChangeHero(target, match, currentResultOfRound, amount * -1, tbsg::EffectChange::Hero_Health);

                target->health -= amount;

                if (receiver->health + amount > receiver->maxHealth)
                {
                    AddChangeHero(receiver, match, currentResultOfRound, receiver->maxHealth - receiver->health, tbsg::EffectChange::Hero_Health);
                    receiver->health = receiver->maxHealth;
                }
                else
                {
                    AddChangeHero(receiver, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Health);
                    receiver->health += amount;
                }
            }
            else
            {
                if (receiver->health + amount > receiver->maxHealth)
                {
                    AddChangeHero(receiver, match, currentResultOfRound, receiver->maxHealth - receiver->health, tbsg::EffectChange::Hero_Health);
                    receiver->health = receiver->maxHealth;
                }
                else
                {
                    AddChangeHero(receiver, match, currentResultOfRound, target->health, tbsg::EffectChange::Hero_Health);
                    receiver->health += target->health;
                }

                AddChangeHero(target, match, currentResultOfRound, target->health * -1, tbsg::EffectChange::Hero_Health);
                int heroID = AddChangeHero(target, match, currentResultOfRound, 0, tbsg::EffectChange::Hero_Death);
                isHeroDead[heroID] = true;
                target->health = 0;
            }
        }
    };
    lua.SetFunction("StealHeroHealth", StealHeroHealth);


    

    
    std::function<int(Hero*)> GetHeroArmor = [](Hero* hero)
    {
        assert(hero != nullptr);
        return hero->armor;
    };
    lua.SetFunction("GetHeroArmor", GetHeroArmor);


    
    std::function<void(Hero*, int)> AddHeroArmor = [match, &currentResultOfRound](Hero* hero, int amount)
    {
        if (hero != nullptr)
        {
            int armor = hero->armor + amount;

            if (armor < 0)
            {
                AddChangeHero(hero, match, currentResultOfRound, hero->armor * -1, tbsg::EffectChange::Hero_Armor);

                hero->armor = 0;
            }
            else
            {
                AddChangeHero(hero, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Armor);

                hero->armor += amount;
            }
        }
    };
    lua.SetFunction("AddHeroArmor", AddHeroArmor);


    
    std::function<void(Hero*, int)> SetHeroArmor = [match, &currentResultOfRound](Hero* hero, int amount)
    {
        if (hero != nullptr)
        {
            int newArmor = amount - hero->armor;
            if (newArmor < 0)
            {
                AddChangeHero(hero, match, currentResultOfRound, 0 - hero->armor, tbsg::EffectChange::Hero_Armor);
                hero->armor = 0;
            }
            else
            {
                AddChangeHero(hero, match, currentResultOfRound, amount - hero->armor, tbsg::EffectChange::Hero_Armor);
                hero->armor = amount;
            }
        }
    };
    lua.SetFunction("SetHeroArmor", SetHeroArmor);


    
    std::function<void(Hero*, Hero*, int)> StealHeroArmor = [match, &currentResultOfRound](Hero* target, Hero* receiver, int amount)
    {
        if (target != nullptr && receiver != nullptr)
        {
            if (target->armor >= amount)
            {
                AddChangeHero(target, match, currentResultOfRound, amount * -1, tbsg::EffectChange::Hero_Armor);
                AddChangeHero(receiver, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Armor);

                target->armor -= amount;
                receiver->armor += amount;
            }
            else
            {
                AddChangeHero(target, match, currentResultOfRound, target->armor * -1, tbsg::EffectChange::Hero_Armor);
                AddChangeHero(receiver, match, currentResultOfRound, target->armor, tbsg::EffectChange::Hero_Armor);

                receiver->armor += target->armor;
                target->armor = 0;
            }
        }
    };
    lua.SetFunction("StealHeroArmor", StealHeroArmor);

    

    
    std::function<int(Hero*)> GetHeroAttack = [](Hero* hero)
    {
        assert(hero != nullptr);
        return hero->attack;
    };
    lua.SetFunction("GetHeroAttack", GetHeroAttack);


    
    std::function<void(Hero*, int)> AddHeroAttack = [match, &currentResultOfRound](Hero* hero, int amount)
    {
        if (hero != nullptr)
        {
            int attack = hero->attack + amount;

            if (attack < 0)
            {
                AddChangeHero(hero, match, currentResultOfRound, hero->attack * -1, tbsg::EffectChange::Hero_Attack);

                hero->attack = 0;
            }
            else
            {
                AddChangeHero(hero, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Attack);

                hero->attack += amount;
            }
        }
    };
    lua.SetFunction("AddHeroAttack", AddHeroAttack);


    
    std::function<void(Hero*, int)> SetHeroAttack = [match, &currentResultOfRound](Hero* hero, int amount)
    {
        if (hero != nullptr)
        {
            if (amount - hero->attack != 0)
                AddChangeHero(hero, match, currentResultOfRound, amount - hero->attack, tbsg::EffectChange::Hero_Attack);

            hero->attack = amount;
        }
    };
    lua.SetFunction("SetHeroAttack", SetHeroAttack);


    
    std::function<void(Hero*, Hero*, int)> StealHeroAttack = [match, &currentResultOfRound](Hero* target, Hero* receiver, int amount)
    {

        if (target != nullptr && receiver != nullptr)
        {
            if (target->attack >= amount)
            {
                AddChangeHero(target, match, currentResultOfRound, amount * -1, tbsg::EffectChange::Hero_Attack);
                AddChangeHero(receiver, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Attack);

                target->attack -= amount;
                receiver->attack += amount;
            }
            else
            {
                AddChangeHero(target, match, currentResultOfRound, target->attack * -1, tbsg::EffectChange::Hero_Attack);
                AddChangeHero(receiver, match, currentResultOfRound, target->attack, tbsg::EffectChange::Hero_Attack);

                receiver->attack += target->attack;
                target->attack = 0;
            }
        }
    };
    lua.SetFunction("StealHeroAttack", StealHeroAttack);

    
    std::function<int(Hero*)> GetHeroTotalAttack = [](Hero* hero)
    {
        assert(hero != nullptr);
        int totalAttack = hero->attack;

        if (hero->weapon != nullptr)
        {
            totalAttack += hero->weapon->attack;
        }

        return totalAttack;
    };
    lua.SetFunction("GetHeroTotalAttack", GetHeroTotalAttack);

    
    
    std::function<Weapon*(Hero*)> GetHeroWeapon = [](Hero* hero)
    {
        assert(hero != nullptr);
        return hero->weapon;
    };
    lua.SetFunction("GetHeroWeapon", GetHeroWeapon);

    
    std::function<void(Hero*, Weapon*)> SetHeroWeapon = [match, &currentResultOfRound](Hero* hero, Weapon* weapon)
    {
        if (hero != nullptr)
        {
            if (weapon != nullptr)
            {
                AddChangeHero(hero, match, currentResultOfRound, weapon->attack, tbsg::EffectChange::Hero_Weapon_Attack);
                AddChangeHero(hero, match, currentResultOfRound, weapon->durability, tbsg::EffectChange::Hero_Weapon_Durability);
            }
            else if (hero->weapon != nullptr)
            {
                AddChangeHero(hero, match, currentResultOfRound, 0 - hero->weapon->attack, tbsg::EffectChange::Hero_Weapon_Attack);
                AddChangeHero(hero, match, currentResultOfRound, 0 - hero->weapon->durability, tbsg::EffectChange::Hero_Weapon_Durability);
            }

            hero->weapon = weapon;
        }
    };
    lua.SetFunction("SetHeroWeapon", SetHeroWeapon);


    
    std::function<void(Hero*, Hero*)> StealHeroWeapon = [match, &currentResultOfRound](Hero* target, Hero* receiver)
    {
        if (target != nullptr && receiver != nullptr)
        {
            if (target->weapon != nullptr)
            {
                AddChangeHero(target, match, currentResultOfRound, target->weapon->attack * -1, tbsg::EffectChange::Hero_Weapon_Attack);
                AddChangeHero(target, match, currentResultOfRound, target->weapon->durability * -1, tbsg::EffectChange::Hero_Weapon_Durability);

                AddChangeHero(receiver, match, currentResultOfRound, target->weapon->attack, tbsg::EffectChange::Hero_Weapon_Attack);
                AddChangeHero(receiver, match, currentResultOfRound, target->weapon->durability, tbsg::EffectChange::Hero_Weapon_Durability);

                receiver->weapon = target->weapon;
                target->weapon = nullptr;
            }
        }
    };
    lua.SetFunction("StealHeroWeapon", StealHeroWeapon);

    
    std::function<Weapon*(int, int)> CreateHeroWeapon = [](int weaponAttack, int weaponDurability)
    {
        Weapon* weapon = new Weapon();
        weapon->attack = weaponAttack;
        weapon->durability = weaponDurability;
        return weapon;
    };
    lua.SetFunction("CreateHeroWeapon", CreateHeroWeapon);



    
    
    std::function<int(Hero*)> GetWeaponAttack = [](Hero* hero)
    {
        if (hero != nullptr && hero->weapon != nullptr)
        {
            return (int)hero->weapon->attack;
        }
        else
        {
            return 0;
        }
    };
    lua.SetFunction("GetWeaponAttack", GetWeaponAttack);

    
    std::function<void(Hero*, int)> AddWeaponAttack = [match, &currentResultOfRound](Hero* hero, int amount)
    {
        if (hero != nullptr && hero->weapon != nullptr)
        {
            int weaponAttack = hero->weapon->attack + amount;

            if (weaponAttack < 0)
            {
                AddChangeHero(hero, match, currentResultOfRound, hero->weapon->attack * -1, tbsg::EffectChange::Hero_Weapon_Attack);

                hero->weapon->attack = 0;
            }
            else
            {
                AddChangeHero(hero, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Weapon_Attack);

                hero->weapon->attack += amount;
            }
        }
    };
    lua.SetFunction("AddWeaponAttack", AddWeaponAttack);


    
    std::function<void(Hero*, int)> SetWeaponAttack = [match, &currentResultOfRound](Hero* hero, int amount)
    {
        if (hero != nullptr && hero->weapon != nullptr)
        {
            AddChangeHero(hero, match, currentResultOfRound, amount - hero->weapon->attack, tbsg::EffectChange::Hero_Weapon_Attack);

            hero->weapon->attack = amount;
        }
    };
    lua.SetFunction("SetWeaponAttack", SetWeaponAttack);

    

    
    std::function<int(Hero*)> GetWeaponDurability = [](Hero* hero)
    {
        if (hero != nullptr && hero->weapon != nullptr)
        {
            return (int)hero->weapon->durability;
        }
        else
        {
            return 0;
        }
    };
    lua.SetFunction("GetWeaponDurability", GetWeaponDurability);


    
    std::function<void(Hero*, int)> AddWeaponDurability = [match, &currentResultOfRound](Hero* hero, int amount)
    {
        if (hero != nullptr && hero->weapon != nullptr)
        {
            int weaponDurability = hero->weapon->durability + amount;

            if (weaponDurability < 0)
            {
                AddChangeHero(hero, match, currentResultOfRound, hero->weapon->durability * -1, tbsg::EffectChange::Hero_Weapon_Durability);
                AddChangeHero(hero, match, currentResultOfRound, 0 - hero->weapon->attack, tbsg::EffectChange::Hero_Weapon_Attack);

                hero->weapon->durability = 0;
                hero->weapon = nullptr;
            }
            else
            {
                AddChangeHero(hero, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Weapon_Durability);

                hero->weapon->durability += amount;
            }
        }
    };
    lua.SetFunction("AddWeaponDurability", AddWeaponDurability);


    
    std::function<void(Hero*, int)> SetWeaponDurability = [match, &currentResultOfRound](Hero* hero, int amount)
    {
        if (hero != nullptr && hero->weapon != nullptr)
        {
            AddChangeHero(hero, match, currentResultOfRound, amount - hero->weapon->durability, tbsg::EffectChange::Hero_Weapon_Durability);
            if (amount == 0)
            {
                AddChangeHero(hero, match, currentResultOfRound, 0 - hero->weapon->attack, tbsg::EffectChange::Hero_Weapon_Attack);
                hero->weapon = nullptr;
            }
            else
            {
                hero->weapon->durability = amount;
            }
        }
    };
    lua.SetFunction("SetWeaponDurability", SetWeaponDurability);


    
    std::function<tbsg::MonsterCard*()> GetCurrentMonster = [match, &currentPlayer]()
    {
        if (match->monsterCards.empty())
        {
            MonsterCard* monsterCard = nullptr;
            return monsterCard;
        }
        return &match->monsterCards[0];
    };
    lua.SetFunction("GetCurrentMonster", GetCurrentMonster);


    
    std::function<Card*(int)> GetPlayedCard = [playedCards, database](int playerID)
    {
        return database->GetCard(playedCards[playerID]);
    };
    lua.SetFunction("GetPlayedCard", GetPlayedCard);


    
    std::function<const Card*(int, int)> GetCardFromHand = [match, database](int playerID, int index)
    {
        const Card* cCard = database->GetCard(match->playerHands[playerID][index]);
        return cCard;
    };
    lua.SetFunction("GetCardFromHand", GetCardFromHand);

    
    std::function<int(int)> GetHandSize = [&match](int playerID)
    {
        return match->playerHands[playerID].size();
    };
    lua.SetFunction("GetHandSize", GetHandSize);

    
    std::function<int(int)> GetDeckSize = [&match](int playerID)
    {
        return match->playerDecks[playerID].size();
    };
    lua.SetFunction("GetDeckSize", GetDeckSize);

    
    std::function<int(int)> GetDiscardSize = [&match](int playerID)
    {
        return match->playerDiscards[playerID].size();
    };
    lua.SetFunction("GetDiscardSize", GetDiscardSize);

    
    std::function<int(int, ptl::string)> GetCardInHandByName = [match, database](int playerID, ptl::string cardName)
    {
        for (size_t i = 0; i < match->playerHands[playerID].size(); ++i)
        {
            if (database->GetCard(match->playerHands[playerID][i])->meta.name == cardName)
            {
                return static_cast<int>(i);
            }
        }
        return -1;
    };
    lua.SetFunction("GetCardInHandByName", GetCardInHandByName);

    
    std::function<int(int, ptl::string)> GetAmountOfCardsInHandByName = [match, database](int playerID, ptl::string cardName)
    {
        int index = 0;
        for (size_t i = 0; i < match->playerHands[playerID].size(); ++i)
        {
            if (database->GetCard(match->playerHands[playerID][i])->meta.name == cardName)
            {
                index++;
            }
        }
        return index;
    };
    lua.SetFunction("GetAmountOfCardsInHandByName", GetAmountOfCardsInHandByName);



    
    std::function<int()> GetCurrentplayerID = [&currentPlayer]()
    {
        return currentPlayer;
    };
    lua.SetFunction("GetCurrentPlayerID", GetCurrentplayerID);

    
    std::function<int()> GetCurrentRivalID = [&currentPlayer]()
    {
        if (currentPlayer == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    };
    lua.SetFunction("GetCurrentRivalID", GetCurrentRivalID);

    
    std::function<void(int, ptl::string)> AddCardToDeckByType = [database, match, &currentResultOfRound](int playerID, ptl::string cardType)
    {
		ptl::vector<unsigned int > cards = database->GetCardsByType(std::move(cardType));

        if (!cards.empty())
        {
	        const unsigned randomIndex = tbsg::gameplay::GetRandomIntInRange(0, cards.size() - 1, match->gameSeed);

            if (cards.size() > randomIndex)
            {
                match->playerDecks[playerID].push_back(cards[randomIndex]);
                gameplay::ShuffleCards(match->playerDecks[playerID]);
                tbsg::Change change{ tbsg::EffectChange::Deck_AddCard, static_cast<int>(cards[randomIndex]), static_cast<unsigned int>(playerID) };
                currentResultOfRound.results.push_back(change);
				cof::Info("[AddCardToDeckByType] added {} to players deck", change.change);
            }

        }else
        {
			cof::Error("[AddCardToDeckByType] could not add card by type because could not find any");
        }
    };
    lua.SetFunction("AddCardToDeckByType", AddCardToDeckByType);

    
    std::function<void(int, ptl::string)> AddCardToDeckByName = [database, match, &currentResultOfRound](int playerID, ptl::string cardName)
    {
        Card* card = database->GetCard(ptl::string(cardName));

        if (card != nullptr)
        {
            match->playerDecks[playerID].push_back(card->id);
            gameplay::ShuffleCards(match->playerDecks[playerID]);
            tbsg::Change change{ tbsg::EffectChange::Deck_AddCard, static_cast<int>(card->id), static_cast<unsigned int>(playerID) };
            currentResultOfRound.results.push_back(change);
			cof::Info("[AddCardToDeckByName] added {} to deck of player {}", cardName, playerID);
        }else
        {
			cof::Error("[AddCardToDeckByName] could not added {} to deck of player {}", cardName, playerID);
        }
    };
    lua.SetFunction("AddCardToDeckByName", AddCardToDeckByName);


    
    std::function<void(int, ptl::string)> AddCardToDiscardByName = [database, match, &currentResultOfRound](int playerID, ptl::string cardName)
    {
        Card* card = database->GetCard(cardName);

        if (card != nullptr)
        {
			auto checkIfinHand = std::find_if(match->playerHands[playerID].begin(), match->playerHands[playerID].end(),[cardId = card->id](const auto id)
			{
				return id == cardId;
			});
			if(checkIfinHand != match->playerHands[playerID].end()){
            match->playerDiscards[playerID].push_back(card->id);
            tbsg::Change change{ tbsg::EffectChange::Discard_AddCard, static_cast<int>(card->id), static_cast<unsigned int>(playerID) };
            currentResultOfRound.results.push_back(change);
			}else
			{
				cof::Warn("[AddCardToDiscardByName] could not remove card from hand which was not there! Therefore we do nothing.");
			}
        }
    };
    lua.SetFunction("AddCardToDiscardByName", AddCardToDiscardByName);

     
     std::function<void(int, ptl::string)> AddCardToHandByType = [database, match, &currentResultOfRound](int playerID, ptl::string cardType)
     {
		 auto cards = std::move(database->GetCardsByType(cardType));
     
         if (!cards.empty())
         {
             int randomIndex = tbsg::gameplay::GetRandomIntInRange(0, cards.size() - 1, match->gameSeed);
             
             if (cards.size() > randomIndex)
             {
				 auto card = cards[randomIndex];
                 match->playerHands[playerID].push_back(card);

                 tbsg::Change change{ tbsg::EffectChange::Hand_AddCard, static_cast<int>(cards[randomIndex]), static_cast<unsigned int>(playerID) };
                 currentResultOfRound.results.push_back(change);
             }

         }
     };
     lua.SetFunction("AddCardToHandByType", AddCardToHandByType);

     
     std::function<void(int, ptl::string)> AddCardToHandByName = [database, match, &currentResultOfRound](int playerID, ptl::string cardName)
     {
         Card* card = database->GetCard(ptl::string(cardName));

         if (card != nullptr)
         {
            match->playerHands[playerID].push_back(card->id);

            tbsg::Change change{ tbsg::EffectChange::Hand_AddCard, static_cast<int>(card->id), static_cast<unsigned int>(playerID) };
            currentResultOfRound.results.push_back(change);
         }
     };
     lua.SetFunction("AddCardToHandByName", AddCardToHandByName);

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    


    
    std::function<void(int, int)> RemoveCardFromHand = [match, &currentResultOfRound, database](int playerID, int index) 
    {
        assert(match != nullptr);
        if (match->playerHands[playerID].size() > index)
        {

            tbsg::Change change{ tbsg::EffectChange::Hand_RemoveCard, static_cast<int>(match->playerHands[playerID][index]), static_cast<unsigned int>(playerID)};
            currentResultOfRound.results.push_back(change);

            match->playerHands[playerID].erase(match->playerHands[playerID].begin() + index);
        }
    };
    lua.SetFunction("RemoveCardFromHand", RemoveCardFromHand);

    
    std::function<void(int)> RemoveAllCardsFromHand = [match, &currentResultOfRound, database](int playerID) 
    {
        assert(match != nullptr);
        for (int i = 0; i < match->playerHands[playerID].size(); ++i)
        {
            tbsg::Change change{ tbsg::EffectChange::Hand_RemoveCard, static_cast<int>(match->playerHands[playerID][i]), static_cast<unsigned int>(playerID) };
            currentResultOfRound.results.push_back(change);

           
        }
		match->playerHands[playerID].clear();
    };
    lua.SetFunction("RemoveAllCardsFromHand", RemoveAllCardsFromHand);

    
    std::function<void(int, ptl::string)> RemoveCardFromHandByName = [match, &currentResultOfRound, database](int playerID, ptl::string cardName) 
    {
        assert(match != nullptr);

        for (size_t i = 0; i < match->playerHands[playerID].size(); ++i)
        {
            if (database->GetCard(match->playerHands[playerID][i])->meta.name == cardName)
            {
                tbsg::Change change{ tbsg::EffectChange::Hand_RemoveCard, static_cast<int>(match->playerHands[playerID][i]), static_cast<unsigned int>(playerID)};
                currentResultOfRound.results.push_back(change);

                match->playerHands[playerID].erase(match->playerHands[playerID].begin() + i);
                break;
            }
        }
    };
    lua.SetFunction("RemoveCardFromHandByName", RemoveCardFromHandByName);

    
    std::function<void(int, ptl::string, int)> RemoveCardsFromHandByName = [&match, &currentResultOfRound, database](int playerID, ptl::string cardName, int amount) 
    {
        assert(match != nullptr);
        int index = 0;
        for (size_t i = 0; i < match->playerHands[playerID].size(); ++i)
        {
            if (database->GetCard(match->playerHands[playerID][i])->meta.name == cardName)
            {
                tbsg::Change change{ tbsg::EffectChange::Hand_RemoveCard, static_cast<int>(match->playerHands[playerID][i]), static_cast<unsigned int>(playerID)};
                currentResultOfRound.results.push_back(change);

                match->playerHands[playerID].erase(match->playerHands[playerID].begin() + i);
                index++;

                if (index == amount)
                    break;
            }
        }
    };
    lua.SetFunction("RemoveCardsFromHandByName", RemoveCardsFromHandByName);

    
    std::function<void(int, ptl::string)> RemoveRandomCardFromHandByName = [match, &currentResultOfRound, database](int playerID, ptl::string cardName)
    {
        assert(match != nullptr);
        ptl::vector<int> cardIndexs;
        for (auto i = 0; i < match->playerHands[playerID].size(); ++i)
        {
            if (database->GetCard(match->playerHands[playerID][i])->meta.name == cardName)
            {
                cardIndexs.push_back(i);
            }
        }

        if (cardIndexs.size() > 0)
        {
            auto randomIndex = tbsg::gameplay::GetRandomIntInRange(0, cardIndexs.size(), match->gameSeed);

            tbsg::Change change{ tbsg::EffectChange::Hand_RemoveCard, static_cast<int>(match->playerHands[playerID][cardIndexs[randomIndex]]), static_cast<unsigned int>(playerID)};
            currentResultOfRound.results.push_back(change);

            match->playerHands[playerID].erase(match->playerHands[playerID].begin() + cardIndexs[randomIndex]);
        }
    };
    lua.SetFunction("RemoveRandomCardFromHandByName", RemoveRandomCardFromHandByName);

    
    std::function<Card*(int, int)> GetDeckCard = [match, database](int playerID, int index) 
    {
        assert(match != nullptr);
        Card* card = database->GetCard(match->playerDecks[playerID][index]);
        return card;
    };
    lua.SetFunction("GetDeckCard", GetDeckCard);

    
    std::function<Card*(int, ptl::string)> GetCardInDeckByName = [match, database](int playerID, ptl::string cardName)
    {
        assert(match != nullptr);
        for (size_t i = 0; i < match->playerDecks[playerID].size(); ++i)
        {
            Card* card = database->GetCard(match->playerDecks[playerID][i]);
            if (card->meta.name == cardName)
            {
                return card;
            }
        }
        Card* emptyCard = nullptr;
        return emptyCard;
    };
    lua.SetFunction("GetCardInDeckByName", GetCardInDeckByName);

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    
    std::function<void(int)> RemoveRandomCardFromDeck = [match, &currentResultOfRound](int playerID) 
    {
	
        assert(match != nullptr);

        int randomIndex = tbsg::gameplay::GetRandomIntInRange(0, match->playerDecks[playerID].size() - 1, match->gameSeed);

        tbsg::Change change{ tbsg::EffectChange::Deck_RemoveCard, static_cast<int>(match->playerDecks[playerID][randomIndex]), static_cast<unsigned int>(playerID)};
        currentResultOfRound.results.push_back(change);

        match->playerDecks[playerID].erase(match->playerDecks[playerID].begin() + randomIndex);

    };
    lua.SetFunction("RemoveRandomCardFromDeck", RemoveRandomCardFromDeck);

    
    std::function<void(int, int)> RemoveRandomCardsFromDeck = [match, &currentResultOfRound](int playerID, int amount) 
    {

        assert(match != nullptr);

        for (int i = 0; i < amount; ++i)
        {
            int randomIndex = tbsg::gameplay::GetRandomIntInRange(0, match->playerDecks[playerID].size() - 1, match->gameSeed);;

            tbsg::Change change{tbsg::EffectChange::Deck_RemoveCard, static_cast<int>(match->playerDecks[playerID][randomIndex]), static_cast<unsigned int>(playerID) };
            currentResultOfRound.results.push_back(change);

            match->playerDecks[playerID].erase(match->playerDecks[playerID].begin() + randomIndex);
        }
    };
    lua.SetFunction("RemoveRandomCardsFromDeck", RemoveRandomCardsFromDeck);


    
    std::function<void(int)> RemoveRandomCardFromHand = [match, &currentResultOfRound, database](int playerID) 
    {

        assert(match != nullptr);
        ptl::vector<Card*> hand;
        for (int i = 0; i < match->playerHands[playerID].size(); ++i)
        {
            hand.push_back(database->GetCard(match->playerHands[playerID][i]));
        }
        if (hand.size() > 0)
        {
            int randomIndex = tbsg::gameplay::GetRandomIntInRange(0, hand.size() - 1, match->gameSeed);

            tbsg::Change change{ tbsg::EffectChange::Hand_RemoveCard, static_cast<int>(match->playerHands[playerID][randomIndex]), static_cast<unsigned int>(playerID) };
            currentResultOfRound.results.push_back(change);

            match->playerHands[playerID].erase(match->playerHands[playerID].begin() + randomIndex);
        }
    };
    lua.SetFunction("RemoveRandomCardFromHand", RemoveRandomCardFromHand);

	std::function<int(int,int)> GetRandomIndex = [match, &currentResultOfRound, database](int begin,int end)
	{
		return  tbsg::gameplay::GetRandomIntInRange(begin, end - 1, match->gameSeed);
	};
	lua.SetFunction("GetRandomIndex", GetRandomIndex);
    
    std::function<void(int, int)> RemoveRandomCardsFromHand = [match, &currentResultOfRound, database](int playerID, int amount)
    {
        assert(match != nullptr);

        ptl::vector<Card*> hand;
        for (int i = 0; i < match->playerHands[playerID].size(); ++i)
        {
            hand.push_back(database->GetCard(match->playerHands[playerID][i]));
        }

        for (int i = 0; i < amount; ++i)
        {
            if (hand.size() > 0)
            {
                int randomIndex = tbsg::gameplay::GetRandomIntInRange(0, hand.size() - 1, match->gameSeed);
                match->playerHands[playerID].erase(match->playerHands[playerID].begin() + randomIndex);

                tbsg::Change change{ tbsg::EffectChange::Hand_RemoveCard, static_cast<int>(match->playerHands[playerID][randomIndex]), static_cast<unsigned int>(playerID) };
                currentResultOfRound.results.push_back(change);
            }
        }
    };
    lua.SetFunction("RemoveRandomCardsFromHand", RemoveRandomCardsFromHand);

    
    std::function<void(int, Card*)> RemoveCardFromDiscard = [match, &currentResultOfRound](int playerID, Card* card)
    {

		auto cardId = card->id;
		auto res = std::find_if(match->playerDiscards[playerID].begin(), match->playerDiscards[playerID].end(),[&cardId](unsigned int id)
		{
			return 	cardId == id;
		});
		if(res != match->playerDiscards[playerID].end())
		{
			tbsg::Change change{ tbsg::EffectChange::Discard_RemoveCard, static_cast<int>(cardId), static_cast<unsigned int>(playerID) };
			match->playerDiscards[playerID].erase(res);
		}
		
    };
    lua.SetFunction("RemoveCardFromDiscard", RemoveCardFromDiscard);


    
    std::function<void(int, int)> RemoveCardFromDeck = [match, &currentResultOfRound](int playerID, int index) 
    {
        assert(match != nullptr);
        if (match->playerDecks[playerID].size() > index);
        {
            tbsg::Change change{ tbsg::EffectChange::Deck_RemoveCard, static_cast<int>(match->playerDecks[playerID][index]), static_cast<unsigned int>(playerID) };
            currentResultOfRound.results.push_back(change);

            match->playerDecks[playerID].erase(match->playerDecks[playerID].begin() + index);
        }
    };
    lua.SetFunction("RemoveCardFromDeck", RemoveCardFromDeck);

    
    std::function<void(int, ptl::string)> RemoveCardFromDeckByName = [match, &currentResultOfRound, database](int playerID, ptl::string cardName) 
    {
        assert(match != nullptr);

        for (int i = 0; i < match->playerDecks[playerID].size(); ++i)
        {
            if (database->GetCard(match->playerDecks[playerID][i])->meta.name == cardName)
            {
                tbsg::Change change{ tbsg::EffectChange::Deck_RemoveCard, static_cast<int>(match->playerDecks[playerID][i]), static_cast<unsigned int>(playerID) };
                currentResultOfRound.results.push_back(change);

                match->playerDecks[playerID].erase(match->playerDecks[playerID].begin() + i);
            }
        }
    };
    lua.SetFunction("RemoveCardFromDeckByName", RemoveCardFromDeckByName);




    
    std::function<void(int, int)> DrawCardFromDeck = [match, database, &currentResultOfRound](int playerID, int amount) 
    {
        assert(match != nullptr && database != nullptr);

        for (size_t i = 0; i < amount; ++i)
        {
            if (match->playerDecks[playerID].size() > 0)
            {
                unsigned int cardID = match->playerDecks[playerID].back();
                match->playerHands[playerID].push_back(std::move(cardID));
                match->playerDecks[playerID].pop_back();

                tbsg::Change change{ tbsg::EffectChange::Deck_DrawCard, static_cast<int>(cardID), static_cast<unsigned int>(playerID) };
                currentResultOfRound.results.push_back(change);
            }
        }
    };
    lua.SetFunction("DrawCardFromDeck", DrawCardFromDeck);

    
    std::function<void(int, int, ptl::string)> DrawNameCardFromDeck = [match, database, &currentResultOfRound](int playerID, int amount, ptl::string name) 
    {
        assert(match != nullptr && database != nullptr);

        auto cardsAdded = 0;

        auto& playerDeck = match->playerDecks[playerID];
        for (auto it = playerDeck.rbegin(); it != playerDeck.rend(); ++it)
        {
            if (cardsAdded < amount)
            {
                unsigned int cardID = *it;
                if (database->GetCard(cardID)->meta.name == name)
                {
                    match->playerHands[playerID].push_back(cardID);
                    playerDeck.erase(--(it.base()));
                    cardsAdded++;

                    tbsg::Change change{ tbsg::EffectChange::Deck_DrawCard, static_cast<int>(cardID), static_cast<unsigned int>(playerID) };
                    currentResultOfRound.results.push_back(change);
                }
            }
            else
            {
                break;
            }
        }
    };
    lua.SetFunction("DrawNameCardFromDeck", DrawNameCardFromDeck);



    
    std::function<MonsterCard*()> GetMonster = [&monsterCards]()
    {
        if (!monsterCards.empty())
        {
            return &monsterCards[0];
        }
        else
        {
            MonsterCard* card = nullptr;
            return card;
        }
    };
    lua.SetFunction("GetMonster", GetMonster);


    
    std::function<int(MonsterCard*)> GetMonsterTrait = [](MonsterCard* card)
    {
        return card->data.monsterTrait;       
    };
    lua.SetFunction("GetMonster", GetMonster);

    

    
    std::function<int(MonsterCard*)> GetCardMaxHealth = [match, currentPlayer](MonsterCard* card)
    {
        if (card == nullptr)
        {
            return match->heroes[!currentPlayer].maxHealth;
        }
        return card->data.maxHealth;
    };
    lua.SetFunction("GetCardMaxHealth", GetCardMaxHealth);


    
    std::function<int(Card*)> GetPlayerCardID = [match, currentPlayer](Card* card)
    {
        return card->id;
    };
    lua.SetFunction("GetPlayerCardID", GetPlayerCardID);

    
    std::function<int(MonsterCard*)> GetMonsterCardID = [match, currentPlayer](MonsterCard* card)
    {
        return card->id;
    };
    lua.SetFunction("GetMonsterCardID", GetMonsterCardID);


    
    std::function<int(MonsterCard*)> GetCardHealth = [match, currentPlayer](MonsterCard* card)
    {
        if (card == nullptr)
        {
            return match->heroes[!currentPlayer].health;
        }
        return card->data.health;
    };
    lua.SetFunction("GetCardHealth", GetCardHealth);

    
    std::function<void(MonsterCard*, int)> SetCardHealth = [&currentResultOfRound, monsterCards, match, currentPlayer, SetHeroHealth](MonsterCard* card, int health)
    {
        if (card != nullptr)
        {
            if (card->data.health != 0)
            {
                AddChangeCard(card, monsterCards,  currentResultOfRound, health - card->data.health, tbsg::EffectChange::Card_Health);
            }

            if (health < 0)
            {
                card->data.health = 0;
            }
            else
            {
                card->data.health = health;
            }
        }
        else
        {
            SetHeroHealth(&match->heroes[!currentPlayer], health);
        }
    };
    lua.SetFunction("SetCardHealth", SetCardHealth);


    
    std::function<void(MonsterCard*, int)> AddCardHealth = [&currentResultOfRound, monsterCards, match, currentPlayer, AddHeroHealth](MonsterCard* card, int health)
    {
        if (card != nullptr)
        {
            int newHealth = card->data.health + health;

            if (newHealth <= 0)
            {
                AddChangeCard(card, monsterCards,  currentResultOfRound, card->data.health * -1, tbsg::EffectChange::Card_Health);

                card->data.health = 0;
            }
            else if (newHealth > card->data.maxHealth)
            {
                AddChangeCard(card, monsterCards, currentResultOfRound, card->data.maxHealth - card->data.health, tbsg::EffectChange::Card_Health);

                card->data.health = card->data.maxHealth;
            }
            else
            {
                AddChangeCard(card, monsterCards,  currentResultOfRound, health, tbsg::EffectChange::Card_Health);

                card->data.health += health;
            }
        }
        else
        {
            AddHeroHealth(&match->heroes[!currentPlayer], health);
        }
    };
    lua.SetFunction("AddCardHealth", AddCardHealth);

    
    std::function<void(MonsterCard*, Hero*, int)> StealCardHealth = [match, &currentResultOfRound, monsterCards, currentPlayer, StealHeroHealth](MonsterCard* target, Hero* receiver, int amount)
    {
        if (target != nullptr && receiver != nullptr)
        {
            if (target->data.health >= amount)
            {
                AddChangeCard(target, monsterCards,  currentResultOfRound, amount * -1, tbsg::EffectChange::Card_Health);

                target->data.health -= amount;

                if (receiver->health + amount > receiver->maxHealth)
                {
                    AddChangeHero(receiver, match, currentResultOfRound, receiver->maxHealth - receiver->health, tbsg::EffectChange::Hero_Health);
                    receiver->health = receiver->maxHealth;
                }
                else
                {
                    AddChangeHero(receiver, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Health);
                    receiver->health += amount;
                }
            }
            else
            {
                if (receiver->health + amount > receiver->maxHealth)
                {
                    AddChangeHero(receiver, match, currentResultOfRound, receiver->maxHealth - receiver->health, tbsg::EffectChange::Hero_Health);
                    receiver->health = receiver->maxHealth;
                }
                else
                {
                    AddChangeHero(receiver, match, currentResultOfRound, target->data.health, tbsg::EffectChange::Hero_Health);
                    receiver->health += target->data.health;
                }

                AddChangeCard(target, monsterCards, currentResultOfRound, target->data.health * -1, tbsg::EffectChange::Card_Health);
                target->data.health = 0;
            }
        }
        else
        {
            StealHeroHealth(&match->heroes[!currentPlayer], receiver, amount);
        }
    };
    lua.SetFunction("StealCardHealth", StealCardHealth);


    
    std::function<int(MonsterCard*)> GetCardArmor = [match, currentPlayer](MonsterCard* card)
    {
        if (card == nullptr)
        {
            return match->heroes[!currentPlayer].health;
        }
        return card->data.armor;
    };
    lua.SetFunction("GetCardArmor", GetCardArmor);

    
    std::function<void(MonsterCard*, int)> SetCardArmor = [&currentResultOfRound, monsterCards, match, currentPlayer, SetHeroArmor](MonsterCard* card, int armor)
    {
        if (card != nullptr)
        {
            if (card->data.armor != 0)
            {
                AddChangeCard(card, monsterCards, currentResultOfRound, armor - card->data.armor, tbsg::EffectChange::Card_Armor);
            }

            if (armor < 0)
            {
                card->data.armor = 0;
            }
            else
            {
                card->data.armor = armor;
            }
        }
        else
        {
            SetHeroArmor(&match->heroes[!currentPlayer], armor);
        }
    };
    lua.SetFunction("SetCardArmor", SetCardArmor);


    
    std::function<void(MonsterCard*, int)> AddCardArmor = [&currentResultOfRound, monsterCards, match, currentPlayer, AddHeroArmor](MonsterCard* card, int armor)
    {
        if (card != nullptr)
        {
            int newArmor = card->data.armor + armor;

            if (newArmor <= 0)
            {
                AddChangeCard(card, monsterCards, currentResultOfRound, card->data.armor * -1, tbsg::EffectChange::Card_Armor);

                card->data.armor = 0;
            }
            else
            {
                AddChangeCard(card, monsterCards, currentResultOfRound, armor, tbsg::EffectChange::Card_Armor);

                card->data.armor += armor;
            }
        }
        else
        {
            AddHeroArmor(&match->heroes[!currentPlayer], armor);
        }
    };
    lua.SetFunction("AddCardArmor", AddCardArmor);

    
    std::function<void(MonsterCard*, Hero*, int)> StealCardArmor = [match, &currentResultOfRound, monsterCards, currentPlayer, StealHeroArmor](MonsterCard* target, Hero* receiver, int amount)
    {
        if (target != nullptr && receiver != nullptr)
        {
            if (target->data.armor >= amount)
            {
                AddChangeCard(target, monsterCards, currentResultOfRound, amount * -1, tbsg::EffectChange::Card_Armor);

                target->data.armor -= amount;

                AddChangeHero(receiver, match, currentResultOfRound, amount, tbsg::EffectChange::Hero_Armor);
                receiver->armor += amount;
            }
            else
            {     
                AddChangeHero(receiver, match, currentResultOfRound, target->data.armor, tbsg::EffectChange::Hero_Armor);
                receiver->health += target->data.armor;
                
                AddChangeCard(target, monsterCards, currentResultOfRound, target->data.armor * -1, tbsg::EffectChange::Card_Armor);
                target->data.armor = 0;
            }
        }
        else if (receiver != nullptr)
        {
            StealHeroArmor(&match->heroes[!currentPlayer], receiver, amount);
        }
    };
    lua.SetFunction("StealCardArmor", StealCardArmor);



    
    std::function<Reward*(MonsterCard*)> GetCardReward = [](MonsterCard* card)
    {
        assert(card != nullptr);
        return card->data.reward[0];
    };
    lua.SetFunction("GetCardReward", GetCardReward);


    
    std::function<bool(Card*, Deck)> HasCard = [](Card* card, Deck deck)
    {
        if (card != nullptr)
        {
            for (size_t i = 0; i < deck.cards.size(); i++)
            {
                if (deck.cards[i]->id == card->id)
                {
                    return true;
                }
            }
        }
        return false;
    };
    lua.SetFunction("HasCard", HasCard);

    
    std::function<Card*(int)> GetCard = [database](unsigned int ID)
    {       
        return database->GetCard(ID);
    };
    lua.SetFunction("GetCard", GetCard);

	std::function<void(ptl::string)> PlaySound = [currentPlayer, &currentResultOfRound](ptl::string soundName)
	{
        tbsg::Change change{ tbsg::EffectChange::Play_Sound, static_cast<int>(HashString(ptl::string(soundName))), static_cast<unsigned int>(currentPlayer) };
		currentResultOfRound.results.push_back(change);
	};

	lua.SetFunction("PlaySound", PlaySound);
}

void tbsg::scripting::AddChangeCard(MonsterCard * card, ptl::vector<tbsg::MonsterCard> monsterCards, ResultOfRound& currentResultOfRound, int amount, EffectChange changeType)
{
	tbsg::Change change;
	change.change = amount;
	change.changeType = changeType;

	for (int cardID = 0; cardID < monsterCards.size(); ++cardID)
	{
		if (&monsterCards[cardID] == card)
		{
			change.index = cardID;
		}
	}
	currentResultOfRound.results.push_back(change);
}

int tbsg::scripting::AddChangeHero(Hero * hero, MatchData * match, ResultOfRound & currentResultOfRound, int amount, EffectChange changeType)
{
    tbsg::Change change;
    change.change = amount;
    change.changeType = changeType;

    if (&match->heroes[0] == hero)
    {
        change.index = 0;
    }
    else
    {
        change.index = 1;
    }

    currentResultOfRound.results.push_back(change);
    return change.index;
}

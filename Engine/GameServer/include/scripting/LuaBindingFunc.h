#pragma once

#include "gameplay/DataStructs.h"
#include "Payloads.h"

namespace tbsg
{
    class GameDataDatabase;
    namespace scripting
    {
        class LuaContext;

        
        void BindMatchFunc(LuaContext& lua, MatchData* match, int& currentPlayer, ptl::vector<tbsg::MonsterCard>& monsterCards, ptl::vector<unsigned int> playedCards, GameDataDatabase* database, ResultOfRound& currentResultOfRound, ptl::vector<bool>& isHeroDead, bool& isAtBossFight, bool& isAtEndFight);

        int AddChangeHero(Hero* hero, MatchData* match, ResultOfRound &currentResultOfRound, int amount, EffectChange changeType);
        void AddChangeCard(MonsterCard* card, ptl::vector<tbsg::MonsterCard> monsterCards, ResultOfRound& currentResultOfRound, int amount, EffectChange changeType);
    }
}

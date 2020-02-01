#pragma once
#include "Payloads.h"
#include <random>

namespace tbsg{
    
    struct MatchData {


		
		ptl::vector<tbsg::MonsterCard> monsterCards{1};

		
		ptl::vector<ptl::vector<unsigned int>> playerDecks{2};
		ptl::vector<ptl::vector<unsigned int>> playerHands{2};
		ptl::vector<ptl::vector<unsigned int>> playerDiscards{2};

        
        ptl::vector<tbsg::Hero> heroes{ 2 };

		
		ptl::vector<unsigned int> playedCards{0,0};

        
        unsigned int playerFightDuration;

		
		unsigned int playerReadyState{ 0 };
		unsigned int nextStartingPlayer{ 0 };
        unsigned int playerAmount{ 2 };

		std::mt19937 gameSeed{0};
		unsigned int seed{ 0 };

        bool isMatchDone{false};
    };
}


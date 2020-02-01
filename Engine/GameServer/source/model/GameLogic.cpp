#include <iostream>
#include <random>
#include "model/GameLogic.h"
#include "scripting/LuaBindingFunc.h"
#include "gameplay/scripting/LuaContext.h"
#include "Payloads.h"
#include "gameplay/scripting/LuaContextHelper.h"
#include "GameDataDatabase.h"
#include "core/StrHash.h"
#include "LoggingFunction.h"
#include "core/Config.h"
#include "ProfileDatabase.h"
#include "gameplay/GamePlayHelper.h"
#include "gameplay/MatchHelper.h"

void tbsg::GameLogic::Initialize(scripting::LuaContext& luaContext)
{
	this->luaContext = &luaContext;

	
	firstPlayerReady = false;
	secondPlayerReady = false;
	games = {};
	playerList = {};
	currentPlayer = -1;
	randomSeed = 0;
}

void tbsg::GameLogic::CreateMatch(unsigned int connectionId, unsigned int lobbyID)
{
    playerList.insert(lobbyID, { connectionId ,0 });
    games.emplace(lobbyID, MatchData{});
    const auto profile = playerDB->GetProfileUsingConnection(connectionId);
    cof::Info("\u001b[33m[GameDataModel]\u001b[0m Match with ID #{} was created by {}", lobbyID, profile->username);
}

void tbsg::GameLogic::DeleteMatch(unsigned, unsigned lobbyId)
{
    games.erase(lobbyId);
    cof::Info("\u001b[31m[GameDataModel]\u001b[0m Deleted match with ID #{} ", lobbyId);
}

void tbsg::GameLogic::SimulateAIMatch(tbsg::MatchData& matchData, const int& amountOfGamesToSimulate)
{
	
    AISystem aiSystem;
    aiSystem.Initialize(gameDataDataBase, 3);

    for (int i = 0; i < amountOfGamesToSimulate; ++i)
    {
        MatchData simulationMatchData = matchData;
    	aiSystem.RunBalancingTool(simulationMatchData);
    std::cout << "SIMULATE=DONE\n";
    }
    std::exit(1);
}

tbsg::ResultOfRound tbsg::GameLogic::OnPlay(unsigned int gameID)
{
	
    return tbsg::ResultOfRound{};
}

 bool tbsg::GameLogic::ArePlayersReady() const
{
	return firstPlayerReady && secondPlayerReady;
}


tbsg::MatchData* tbsg::GameLogic::GetMatch(int gameID)
{
    auto search = games.find(gameID);
    if (search != games.end()) {
        return &search->second;
    }
    return nullptr;
}

const std::unordered_map<int, tbsg::MatchData>& tbsg::GameLogic::GetMatches() const
{
    return  games;
}


const std::pair<tbsg::Profile*, tbsg::Profile*> tbsg::GameLogic::GetProfiles(unsigned lobbyID) const
{
    auto& pair = playerList.at(lobbyID);
	return { playerDB->GetProfile(pair.first), playerDB->GetProfile(pair.second) };
}

std::vector<std::vector<tbsg::Card>> tbsg::GameLogic::GetPlayedCards(tbsg::Play& play)
{
	std::vector<std::vector<tbsg::Card>> playedCards;
	std::vector<tbsg::Card> playedCardsPlayer1;
	std::vector<tbsg::Card> playedCardsPlayer2;

	playedCards.push_back(playedCardsPlayer1);
	playedCards.push_back(playedCardsPlayer2);

	return std::vector<std::vector<tbsg::Card>>();
}

void tbsg::GameLogic::PrintStats(ptl::vector<Hero> heroes, tbsg::MonsterCard* monster)
{
    for (int i = 0; i < heroes.size(); ++i)
    {
      cof::Debug(("Hero" + std::to_string(i) + " Stats:").c_str());

        std::cout << "HeroHealth: " << heroes[i].health << std::endl;
        std::cout << "HeroArmor: " << heroes[i].armor << std::endl;
        std::cout << "HeroAttack: " << heroes[i].attack << std::endl;
        std::cout << "HeroResource: " << heroes[i].resource << std::endl;
    }

    cof::Debug("Monster Stats: ");
    std::cout << "MonsterHealth: "<< monster->data.health << std::endl;
    std::cout << "MonsterArmor: " << monster->data.armor << std::endl;

}
void tbsg::GameLogic::PrintMatchStats(MatchData & matchData)
{
    std::cout << std::endl;

    for (int i = 0; i < matchData.heroes.size(); ++i)
    {
        cof::Info(("Player " + std::to_string(i + 1) + " Stats: ").c_str());

        cof::Info(("HeroHealth " + std::to_string(matchData.heroes[i].health)).c_str());
        cof::Info(("HeroArmor " + std::to_string(matchData.heroes[i].armor)).c_str());
        cof::Info(("HeroAttack " + std::to_string(matchData.heroes[i].attack)).c_str());
        cof::Info(("HeroResource " + std::to_string(matchData.heroes[i].resource)).c_str());

        cof::Info(("DeckSize: " + std::to_string(matchData.playerDecks[i].size())).c_str());
        cof::Info(("DiscardSize: " + std::to_string(matchData.playerDiscards[i].size())).c_str());
        cof::Info(("HandSize: " + std::to_string(matchData.playerHands[i].size())).c_str());

        ptl::string cards;
        for(unsigned int cardsIndex : matchData.playerHands[i])
        {
            cards = cards + gameDataDataBase->GetCard(cardsIndex)->meta.name + " ID: " + ptl::to_string(gameDataDataBase->GetCard(cardsIndex)->id) + " / ";
        }
        cof::Info(("Hand: " + cards).c_str());
        std::cout << std::endl;
    }

    cof::Info("Monster Stats: ");
    cof::Info(("MonsterHealth: " + std::to_string(matchData.monsterCards[0].data.health)).c_str());
    cof::Info(("MonsterArmor: " + std::to_string(matchData.monsterCards[0].data.armor)).c_str());
    std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;

}

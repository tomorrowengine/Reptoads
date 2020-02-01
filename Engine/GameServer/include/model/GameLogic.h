#pragma once
#include "gameplay/DataStructs.h"
#include <unordered_map>
#include "AISystem.h"
#include "core/SparseSet.h"


namespace tbsg{
    struct Play;
    struct Profile;
	struct MatchData;
    class GameDataDatabase;
    class ProfileDatabase;
    namespace  scripting
    {
        class LuaContext;
    }
 
class GameLogic
{
public:
    explicit GameLogic(GameDataDatabase& gameData, ProfileDatabase& playerDB)
        : gameDataDataBase(&gameData), playerDB(&playerDB)
    {
    }

    GameLogic(const GameLogic& other) = default;
    GameLogic(GameLogic&& other) = default;

    void Initialize(scripting::LuaContext& luaContext);
    
    
    void CreateMatch(unsigned int connectionId, unsigned int lobbyId);
    void DeleteMatch(unsigned int connectionId, unsigned int lobbyId);

	void SimulateAIMatch(tbsg::MatchData& matchData, const int& amountOfGamesToSimulate);

	ResultOfRound OnPlay(unsigned int gameID);
    

    [[nodiscard]] bool ArePlayersReady() const;

	
    tbsg::MatchData* GetMatch(int gameID);
    const std::unordered_map<int, tbsg::MatchData>& GetMatches() const;
    const std::pair<tbsg::Profile*, tbsg::Profile*> GetProfiles(unsigned int lobbyID) const;
    int GetCurrentPlayer() { return currentPlayer; };
    int GetRivalPlayer() { return currentPlayer > 0 ? currentPlayer - 1 : currentPlayer + 1; };
	void SetSeed(unsigned int seed) { randomSeed = seed; }

	bool firstPlayerReady = false;
	bool secondPlayerReady = false;
private:
    std::vector<std::vector<tbsg::Card>> GetPlayedCards(tbsg::Play& play);
    void PrintStats(ptl::vector<Hero> heroes, MonsterCard* monster);
    void PrintMatchStats(MatchData& matchData);

    GameDataDatabase* gameDataDataBase;
    ProfileDatabase* playerDB;
    scripting::LuaContext* luaContext;
    const ptl::string luaPlayFunctionName = "OnCardPlay";
    std::unordered_map<int, tbsg::MatchData> games;
    ptl::sparse_set<unsigned int,std::pair<unsigned int, unsigned int>> playerList;
    
    int currentPlayer = -1;
	unsigned int randomSeed = 0;
};
}

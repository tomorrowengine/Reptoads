#include "model/AISystem.h"
#include "gameplay/DataStructs.h"
#include "gameplay/scripting/LuaContext.h"
#include "Payloads.h"
#include "GameDataDatabase.h"
#include "gameplay/MatchHelper.h"
#include "catch/catch.hpp"

#include <rapidjson/document.h>
#include "core/Config.h"
#include <rapidjson/filereadstream.h>
#include "gameplay/GamePlayHelper.h"
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include "LoggingFunction.h"
#include "core/Assertion.h"

#ifndef errno_t
typedef int errno_t;
#endif

#ifdef __unix
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),  (mode)))==NULL
#endif

void tbsg::AISystem::Initialize(GameDataDatabase* gameDataModel, size_t amountOfPlaysToStore)
{
	// Load the utility formulas from the Json file
    LoadUtilityFormulas();

	// Store a pointer to the gameDataModel so that we can easily access this later
    gameDataModelRef = gameDataModel;
    numberOfPlaysToStore = amountOfPlaysToStore;
}

void tbsg::AISystem::RunBalancingTool(MatchData& matchData)
{
	gameplay::ShuffleCards(matchData.playerDecks[0]);
	gameplay::ShuffleCards(matchData.playerDecks[1]);

	//If the hands are empty, we expect that this means we should use the default amount of cards
    if(matchData.playerHands[0].empty())
    {
        tbsg::gameplay::DrawCards(matchData.playerDecks[0], matchData.playerHands[0], 4);
    }
    if (matchData.playerHands[1].empty())
    {
        tbsg::gameplay::DrawCards(matchData.playerDecks[1], matchData.playerHands[1], 4);
    }

	// Run the simulation with the given matchdata
    SimulateGame(matchData);
}

unsigned tbsg::AISystem::RunGameAI(MatchData& matchData, unsigned int aiPlayerID)
{
    unsigned int selectedCard = 0;

	// Run the simulations for all the cards in that the player has and calculate their utility scores
    ptl::vector<PlayedCard> cardsAI = PickCards(matchData, aiPlayerID);

	// Get the card with the highest utility score
    selectedCard = GetBestCard(cardsAI);

	// The selected card should not be able to be none since the player always draws a card at the start of a turn
    if(selectedCard == 0)
    {
        cof::Error("No card was selected by the AI!");
        if (matchData.playerHands[aiPlayerID].empty())
        {
            cof::Error("AI Player's hand was empty!");
        }
        else
        {
            PrintDeck(matchData.playerHands[aiPlayerID], "Hand");
        }
        PrintDeck(matchData.playerDecks[aiPlayerID], "Deck");
        PrintDeck(matchData.playerDiscards[aiPlayerID], "Discard");
    }

    return selectedCard;
}

void tbsg::AISystem::SimulateGame(MatchData& simulationMatchData)
{
    GameInfo gameInfo;
    scripting::LuaContext luaContext;
    luaContext.Initialize();

    int turnCounter = 0;
    
	// Keep the game running as long at the match is not complete and the turn limit has not been reached.
    while(!simulationMatchData.isMatchDone && (turnCounter < 1000 || !limitTurnAmount))
    {
        ++turnCounter;
		cof::Info("Turn {}", turnCounter);
        unsigned int firstCard = 0;
        unsigned int secondCard = 0;
		
		// Let both of the AIs simulate each card in their hands to calcuate the utility scores for them.
        ptl::vector<PlayedCard> cardsFirstAI = PickCards(simulationMatchData, 0);
        ptl::vector<PlayedCard> cardsSecondAI = PickCards(simulationMatchData, 1);

		// If the AIs have health left, get the best card. Otherwise set create a temporary card with the name "No card selected", 
		// this makes it clear in the data gathered that it did not pick a card
        if (simulationMatchData.heroes[0].health > 0)
        {
            firstCard = GetBestCard(cardsFirstAI);
        }
        else
        {
            Card tempCard;
            tempCard.meta.name = "No card selected";
            for (auto& playedCard : cardsFirstAI)
            {
                playedCard.selectedCard = tempCard;
            }
        }

        if (simulationMatchData.heroes[1].health > 0)
        {
            secondCard = GetBestCard(cardsSecondAI);
        }
        else
        {
            Card tempCard;
            tempCard.meta.name = "No card selected";
            for (auto& playedCard : cardsSecondAI)
            {
                playedCard.selectedCard = tempCard;
            }
        }
		
		// Set the selected cards as the played cards in the match data and play out the turn
        simulationMatchData.playedCards = ptl::vector<unsigned int>{firstCard, secondCard};
        ResultOfRound result = gameplay::OnPlay(simulationMatchData, luaContext, gameDataModelRef);


		// Collect the important data from the turn that played out and add it to the game info.
        ptl::vector<PlayerChanges> changes { 2 };
        GetChangeData(result, changes);

        TurnInfo turnInfo{ simulationMatchData, std::make_pair(cardsFirstAI, cardsSecondAI), changes};
        gameInfo.turnInfo.push_back(turnInfo);
    }

	// Once the match is done, store all the gathered data in a Json file, clear the information gathered and deallocate the created luacontext
    StoreGatheredGameData(gameInfo);
    gameInfo.turnInfo.clear();
    luaContext.Deallocate();
}

ptl::vector<tbsg::PlayedCard> tbsg::AISystem::PickCards(MatchData& simulationMatchData, unsigned int playerID)
{
	// Create the futures that will be used to run the simulations in
    ptl::vector<std::future<PlayedCard>> futures;
    ptl::vector<tbsg::PlayedCard> simulationResults;

    assert(!simulationMatchData.playerHands[playerID].empty() && "Hand is empty should not be possible");
    
	// Run an async simulation for each card in the hand of the player to calculate its utility score.
    for (auto& selectedCard : simulationMatchData.playerHands[playerID])
    {
		futures.push_back(std::async(std::launch::async, [&] { return AISystem::SimulateTurn(simulationMatchData, playerID, selectedCard); }));
    }
    
	// Wait for the simulationsto be done and get the results
    for (auto& currentFuture : futures)
    {
        if (!currentFuture.valid()) { continue; }
        PlayedCard result = currentFuture.get();
        simulationResults.push_back(result);
    }
    futures.clear();

	// Get the cards with the highest utility scores
	ptl::vector<PlayedCard> bestCards = GetTopCards(simulationResults, simulationMatchData);
    
    return bestCards;
}

tbsg::PlayedCard tbsg::AISystem::SimulateTurn(MatchData& simulationMatch, unsigned int playerID, unsigned int selectedCard)
{
	// Set up a dummy match, this has to be a copy since we just want to run some simulations on it without changing the actual match data
    MatchData dummyMatch = simulationMatch;
    PlayedCard cardInfo;

	// Create a new lua context, we have to make a new one each time since we are running on multiple threads at the same time
    scripting::LuaContext luaContext;

    luaContext.Initialize();

    dummyMatch.playedCards[playerID] = selectedCard;
    dummyMatch.playedCards[!playerID] = 0;

	// Gather the data that we want to store
    if(selectedCard != 0)
    {
        cardInfo.selectedCard = *gameDataModelRef->GetCard(selectedCard);
    }
    else
    {
        Card emptyCard;
        emptyCard.meta.name = "No Card Played";
        emptyCard.id = 0;
    
        cardInfo.selectedCard = emptyCard;
    }
    cardInfo.matchDataBefore = dummyMatch;
    const ResultOfRound results = gameplay::OnPlay(dummyMatch, luaContext, gameDataModelRef);

    GetChangeData(results, cardInfo.playerChanges);
    
    cardInfo.matchDataAfter = dummyMatch;

	// Calculate the utility score for the played card based on the matchdata before and after
    cardInfo.utilityScore = CalculateUtility(dummyMatch, simulationMatch, playerID);

    luaContext.Deallocate();
	return cardInfo;
}

ptl::vector<tbsg::PlayedCard> tbsg::AISystem::GetTopCards(ptl::vector<PlayedCard>& playedCards, MatchData& simulationMatchData)
{
    ptl::vector<PlayedCard> topCards;
    float lowestUtilityScore = 1.f;

    for (auto& currentCard : playedCards)
    {
		// Check if the top cards already stores the desired amount of cards
        if (topCards.size() >= numberOfPlaysToStore)
        {
			// Check if the current card has a higher utility score than the lowest one stored
            if (currentCard.utilityScore > lowestUtilityScore)
            {
				// If the utility score is highre, search for the card that has the lower utility score and set that to the new card
                for (auto& currentTopCard : topCards)
                {
                    if (currentTopCard.utilityScore == lowestUtilityScore)
                    {
                        currentTopCard = currentCard;
                        break;
                    }
                }
				
				//Then loop over the new top cards and check what the new lowest score is
                for (auto& currentTopCard : topCards)
                {
                    lowestUtilityScore = 1.f;
                    if (currentTopCard.utilityScore < lowestUtilityScore)
                    {
                        lowestUtilityScore = currentTopCard.utilityScore;
                    }
                }
            }
        }
        else
        {
			// Since we do not already store the desired amount of cards, we can just add the current one
            topCards.push_back(currentCard);

			// See if this card has a lower utility score than the previous lowest
            if (currentCard.utilityScore < lowestUtilityScore)
            {
                lowestUtilityScore = currentCard.utilityScore;
            }
        }
    }
    return topCards;
}

unsigned tbsg::AISystem::GetBestCard(ptl::vector<PlayedCard>& topCards)
{
    float highestUtilityScore = 0.f;
    PlayedCard* bestCard = nullptr;

	// Look for the card with the highest utility score
    for(auto& currentCard : topCards)
    {
        if(currentCard.utilityScore >= highestUtilityScore)
        {
            highestUtilityScore = currentCard.utilityScore;
            bestCard = &currentCard;
        }
    }

	// Set the best card to the card that was played and return the ID
    if(bestCard)
    {  
        bestCard->isCardPlayed = true;
        return bestCard->selectedCard.id;
    }

    return 0;
}

float tbsg::AISystem::CalculateUtility(MatchData& matchDummy, MatchData& originalMatchData, unsigned int playerID)
{
	// Create a vector for the utility scores and reserve 5 spots since we expect that there will be at least 5 scores added
    ptl::vector<float> utilityScores;
    utilityScores.reserve(5);
    
	// Add a base score of 0.5
    utilityScores.push_back(0.5f);

    const unsigned int opponentID = !playerID;

	// If there was no card selected, add a bad score since in general not playing a card is bad
    if (matchDummy.playedCards[playerID] == 0)
    {
        utilityScores.push_back(0.0f);
        utilityScores.push_back(0.25f);
    }
    
	// Check the difference in health of the playing AI and add utility scores accordingly
    int statDifference = matchDummy.heroes[playerID].health - originalMatchData.heroes[playerID].health;
    if (statDifference != 0)
    {
		// The entire goal of the game is to stay alive, so add a bunch of really bad utility scores if the AI died because of the card
        if (matchDummy.heroes[playerID].health <= 0)
        {
            utilityScores.emplace_back(0.f);
            utilityScores.emplace_back(0.f);
            utilityScores.emplace_back(0.f);
        }
        statDifference += utilityFunctions[static_cast<int>(UtilityFunctions::Player_Health)].second * 0.5;
        utilityScores.push_back(utilityFunctions[static_cast<int>(UtilityFunctions::Player_Health)].first(static_cast<float>(statDifference)));
    }
    
	// Check the difference in armor of the playing AI and add utility scores accordingly
    statDifference = matchDummy.heroes[playerID].armor - originalMatchData.heroes[playerID].armor;
    if (statDifference != 0)
    {
        statDifference += utilityFunctions[static_cast<int>(UtilityFunctions::Player_Armor)].second * 0.5;
        utilityScores.push_back(utilityFunctions[static_cast<int>(UtilityFunctions::Player_Armor)].first(static_cast<float>(statDifference)));
    }

	// Check the difference in the cards in the hand of the playing AI and add utility scores accordingly
    size_t cardDifference = matchDummy.playerHands[playerID].size() - originalMatchData.playerHands[playerID].size();
    if (cardDifference != 0)
    {
        cardDifference += utilityFunctions[static_cast<int>(UtilityFunctions::Player_Hand)].second * 0.5;
        utilityScores.push_back(utilityFunctions[static_cast<int>(UtilityFunctions::Player_Hand)].first(static_cast<float>(cardDifference)));
    }
    
	// Check the difference in the amount of cards in the deck of the playing AI and add utility scores accordingly
    cardDifference = matchDummy.playerDecks[playerID].size() - originalMatchData.playerDecks[playerID].size();
    if (cardDifference != 0)
    {
        cardDifference += utilityFunctions[static_cast<int>(UtilityFunctions::Player_Deck)].second * 0.5;
        utilityScores.push_back(utilityFunctions[static_cast<int>(UtilityFunctions::Player_Deck)].first(static_cast<float>(cardDifference)));
    }

	// Check the difference in health of the opponent AI and add utility scores accordingly
    statDifference = matchDummy.heroes[opponentID].health - originalMatchData.heroes[opponentID].health;
    if (statDifference != 0)
    {
        statDifference += utilityFunctions[static_cast<int>(UtilityFunctions::Opponent_Health)].second * 0.5;
        utilityScores.push_back(utilityFunctions[static_cast<int>(UtilityFunctions::Opponent_Health)].first(static_cast<float>(statDifference)));
    }

	// Check the difference in armor of the opponent AI and add utility scores accordingly
    statDifference = matchDummy.heroes[opponentID].armor - originalMatchData.heroes[opponentID].armor;
    if (statDifference != 0)
    {
        statDifference += utilityFunctions[static_cast<int>(UtilityFunctions::Opponent_Armor)].second * 0.5;
        utilityScores.push_back(utilityFunctions[static_cast<int>(UtilityFunctions::Opponent_Armor)].first(static_cast<float>(statDifference)));
    }
    
	// Check the difference in the cards in the hand of the opponent AI and add utility scores accordingly
    cardDifference = matchDummy.playerHands[opponentID].size() - originalMatchData.playerHands[opponentID].size();
    if (cardDifference != 0)
    {
        cardDifference += utilityFunctions[static_cast<int>(UtilityFunctions::Opponent_Hand)].second * 0.5;
        utilityScores.push_back(utilityFunctions[static_cast<int>(UtilityFunctions::Opponent_Hand].first(static_cast<float>(cardDifference)));
    }

	// Check the difference in the amount of cards in the deck of the opponent AI and add utility scores accordingly
    cardDifference = matchDummy.playerDecks[opponentID].size() - originalMatchData.playerDecks[opponentID].size();
    if (cardDifference != 0)
    {
        cardDifference += utilityFunctions[static_cast<int>(UtilityFunctions::Opponent_Deck)].second * 0.5;
        utilityScores.push_back(utilityFunctions[static_cast<int>(UtilityFunctions::Opponent_Deck)].first(static_cast<float>(cardDifference)));
    }

	// Check if there are still monsters left to fight
    if(!matchDummy.monsterCards.empty())
    {
		// See if a monster has died
        if(matchDummy.monsterCards.size() == originalMatchData.monsterCards.size())
        {
			// Check the difference in the health of the monster and add utility scores accordingly
            statDifference = matchDummy.monsterCards[0].data.health - originalMatchData.monsterCards[0].data.health;
            if(statDifference != 0)
            {
                statDifference += utilityFunctions[static_cast<int>(UtilityFunctions::Monster_Health].second * 0.5;
                utilityScores.push_back(utilityFunctions[static_cast<int>(UtilityFunctions::Monster_Health].first(static_cast<float>(statDifference)));
            }

			// Check the armor of the monster and add utility scores accordingly
            statDifference = matchDummy.monsterCards[0].data.armor - originalMatchData.monsterCards[0].data.armor;
            if (statDifference != 0)
            {
                statDifference += utilityFunctions[static_cast<int>(UtilityFunctions::Monster_Armor)].second * 0.5;
                utilityScores.push_back(utilityFunctions[static_cast<int>(UtilityFunctions::Monster_Armor)].first(static_cast<float>(statDifference)));
            }
        }
        else
        {
			// If the monster died, add a high utility score
            utilityScores.push_back(1.f);
        }
    }

	// Return the average utilityscore
    return AddUtilityScores(utilityScores);
}

void tbsg::AISystem::LoadUtilityFormulas()
{
	// Open the json file that contains all the utility function setting
    rapidjson::GenericDocument< rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> > aiDoc;
    char readBuffer[512];

    FILE* fp;
    errno_t err = fopen_s(&fp, tbsg::Config::Get().aiFile.c_str(), "r");
    ASSERT_MSG(!err, "Could not load config");


    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    aiDoc.ParseStream(is);

    assert(aiDoc.IsObject());

    for (auto& StatObject : aiDoc.GetObject())
    {
        assert(StatObject.value.IsObject());
        assert(StatObject.value["AI"].IsObject());
        for (auto& HeroObject : StatObject.value.GetObject())
        {
            assert(HeroObject.value.IsObject());

            assert(HeroObject.value["GraphType"].IsInt());
            assert(HeroObject.value["Arguments"].IsArray());

            const int graphType = HeroObject.value["GraphType"].GetInt();
            switch (graphType)
            {
            case 0:
            {
                assert(HeroObject.value["Arguments"].GetArray().Size() > 1);
                assert(HeroObject.value["Arguments"][0].IsBool());
                assert(HeroObject.value["Arguments"][1].IsFloat());

                bool shouldGraphFlip = HeroObject.value["Arguments"][0].GetBool();
                float maxRange = HeroObject.value["Arguments"][1].GetFloat();

				// Add a lambda for a linear line formula to calculate the current variable (e.g. heal change)
                const std::function<float(float)> function = 
                [maxRange, shouldGraphFlip]
                (float a_Variable) -> float
                {
                    float utilityScore = a_Variable / maxRange;

                    if (utilityScore > 1.f)
                    {
                        utilityScore = 1.f;
                    }
                    
                    if (shouldGraphFlip)
                    {
                        utilityScore = 1 - utilityScore;
                    }

                    return utilityScore;
                };

                utilityFunctions.emplace_back(function, maxRange);
                break;
            }
            case 1:
            {
                assert(HeroObject.value["Arguments"].GetArray().Size() > 1);
                assert(HeroObject.value["Arguments"][0].IsBool());
                assert(HeroObject.value["Arguments"][1].IsFloat());
                assert(HeroObject.value["Arguments"][2].IsFloat());

                bool shouldGraphFlip = HeroObject.value["Arguments"][0].GetBool();
                float maxRange = HeroObject.value["Arguments"][1].GetFloat();
                float steepness = HeroObject.value["Arguments"][2].GetFloat();

				// Add a lambda for a quadratic curve formula to calculate the current variable (e.g. heal change)
                const std::function<float(float)> function = 
                [maxRange, shouldGraphFlip, steepness]
                (float a_Variable) -> float
                {
                    float utilityScore = powf(a_Variable / maxRange, steepness);

                    
                    if (utilityScore > 1.f)
                    {
                        utilityScore = 1.f;
                    }

                    
                    if (shouldGraphFlip)
                    {
                        utilityScore = 1 - utilityScore;
                    }

                    return utilityScore;
                };

                utilityFunctions.emplace_back(function, maxRange);
                break;
            }
            case 2:
            {
                assert(HeroObject.value["Arguments"].GetArray().Size() > 1);
                assert(HeroObject.value["Arguments"][0].IsBool());
                assert(HeroObject.value["Arguments"][1].IsFloat());
                assert(HeroObject.value["Arguments"][2].IsFloat());

                bool shouldGraphFlip = HeroObject.value["Arguments"][0].GetBool();
                float maxRange = HeroObject.value["Arguments"][1].GetFloat();
                float steepness = HeroObject.value["Arguments"][2].GetFloat();

				// Add a lambda for a piecewise sigmoid curve formula to calculate the current variable (e.g. heal change)
                const std::function<float(float)> function =
                [maxRange, shouldGraphFlip, steepness]
                (float a_Variable) -> float
                {
                    float utilityScore = 1 / (1 + powf(steepness, -(a_Variable / maxRange * 12) + 6));
                    
                    if (utilityScore > 1.f)
                    {
                        utilityScore = 1.f;
                    }

                    
                    if (shouldGraphFlip)
                    {
                        utilityScore = 1 - utilityScore;
                    }

                    return utilityScore;
                };

                utilityFunctions.emplace_back(function, maxRange);
                break;
            }
            case 3:
            {
                assert(HeroObject.value["Arguments"].GetArray().Size() > 1);
                assert(HeroObject.value["Arguments"][0].IsFloat());
                assert(HeroObject.value["Arguments"][1].IsFloat());
                assert(HeroObject.value["Arguments"][2].IsFloat());
                assert(HeroObject.value["Arguments"][3].IsFloat());
                assert(HeroObject.value["Arguments"][4].IsFloat());
                assert(HeroObject.value["Arguments"][5].IsFloat());
                assert(HeroObject.value["Arguments"][6].IsFloat());

                float startingHeight = HeroObject.value["Arguments"][0].GetFloat();
                float height1 = HeroObject.value["Arguments"][1].GetFloat();
                float height2 = HeroObject.value["Arguments"][2].GetFloat();
                float height3 = HeroObject.value["Arguments"][3].GetFloat();
                float height4 = HeroObject.value["Arguments"][4].GetFloat();
                float height5 = HeroObject.value["Arguments"][5].GetFloat();
                float maxRange = HeroObject.value["Arguments"][6].GetFloat();

				// Add a lambda for a piecewise linear line formula to calculate the current variable (e.g. heal change)
				// A pievewise linear function is the most customizable function that we support, the user can set 6 points between which a linear formula is used. 
                const std::function<float(float)> function =
                [startingHeight, height1, height2, height3, height4, height5, maxRange]
                (float a_Variable) -> float
                {
                    float utilityScore = 0.f;
                    const float range = maxRange / 5;

					// First segment
                    if (a_Variable < range)                     
                    {
                        float s = (height1 - startingHeight) / range;

                        utilityScore = s * a_Variable + startingHeight;
                    }
					// Second segment
                    else if (a_Variable < range * 2)   
                    {
                        float s = (height2 - height1) / range;
                        float x = a_Variable - range;

                        utilityScore = s * x + height1;
                    }
					// Third segment
                    else if (a_Variable < range * 3)            
                    {
                        float s = (height3 - height2) / range;
                        float x = a_Variable - range * 2;

                        utilityScore = s * x + height2;
                    }
					// Fourth segment
                    else if (a_Variable < range * 4)            
                    {
                        float s = (height4 - height3) / range;
                        float x = a_Variable - range * 3;

                        utilityScore = s * x + height3;
                    }
					// Fifth segment
                    else if (a_Variable <= maxRange)          
                    {
                        float s = (height5 - height4) / range;
                        float x = a_Variable - range * 4;

                        utilityScore = s * x + height4;
                    }
					// Out of range
                    else
                    {
                        utilityScore = 1.f;
                    }

                    // Handle edge cases where the utility score is higher or lower than possible
                    if (utilityScore > 1.f)
                    {
                        utilityScore = height5;
                    }
                    else if (utilityScore < 0.f)
                    {
                        utilityScore = startingHeight;
                    }

                    return utilityScore;
                };

                utilityFunctions.push_back(std::make_pair(function, maxRange));
                break;
            }
            default:
            {
                throw std::runtime_error(&"One of the selected graph types does not exist, selected graph type is:"[graphType]);
                break;
            }
            }

        }
    }
	// Close the file
    fclose(fp);
}

float tbsg::AISystem::AddUtilityScores(ptl::vector<float>& utilityScores)
{
    float sumUtilityScores      = 0.f;
    int positiveScoreCounter    = 0;
    int negativeScoreCounter    = 0;

	// Add up all the utilityscores and keep track of how many positive/negative effects there are
    for (float utilityScore : utilityScores)
    {
        sumUtilityScores += utilityScore;

        if(utilityScore > 0.55f)
        {
            ++positiveScoreCounter;
        }
        else if(utilityScore < 0.45f)
        {
            ++negativeScoreCounter;
        }
    }

	// effectCountScore makes sure to add a positive score if there are overall more positive effects or a negative score if there are more negative effects
    const float effectCountScore = (positiveScoreCounter - negativeScoreCounter) / 100.f;

	// Calculate the average score + the effectCountScore
    float result = sumUtilityScores / utilityScores.size() + effectCountScore;

    // Handle edge cases where the score is higher or lower than possible
    if(result > 1.f)
    {
        result = 1.f;
    }
    else if (result < 0.f)
    {
        result = 0.f;
    }

    return result;
}

void tbsg::AISystem::PrintDeck(const ptl::vector<unsigned>& deck, const ptl::string& deckType)
{
	// A utility function that iterates through all the cards inthe deck and prints the names of the cards
    ptl::string gameInfo;
    for (auto& card : deck)
    {
        gameInfo = deckType + ": Card = \"" + gameDataModelRef->GetCard(card)->meta.name + "\" ID = \"" + ptl::to_string(card);
        cof::Info(gameInfo.c_str());
    }
    std::cout << "\n\n";
}

void tbsg::AISystem::GetChangeData(const ResultOfRound& results, ptl::vector<PlayerChanges>& playerChanges)
{
    for (auto& currentChange : results.results)
    {
        const int index = currentChange.index;
        switch (currentChange.changeType)
        {
            case EffectChange::Hero_Health :
            {
                if (currentChange.change < 0)
                {
                    playerChanges[index].damageTaken += -currentChange.change;
                }
                else
                {
                    playerChanges[index].healingDone += currentChange.change;
                }
                break;
            }
            case EffectChange::Hero_Armor :
            {
                if(currentChange.change < 0)
                {
                    playerChanges[index].armorLost += -currentChange.change;
                }
                else
                {
                    playerChanges[index].armorGained += currentChange.change;
                }
                break;
            }
            default :
            {
                break;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
//	Everything below this point is for storing the matchdata in a Json file	//
//////////////////////////////////////////////////////////////////////////////

void tbsg::AISystem::StoreGatheredGameData(const GameInfo& gameInfo)
{
    rapidjson::Document simulationDataDoc;
    simulationDataDoc.Parse("{}");

    rapidjson::Value turnsArray;
    turnsArray.SetArray();
    turnsArray.Reserve(static_cast<rapidjson::SizeType>(gameInfo.turnInfo.size()), simulationDataDoc.GetAllocator());
    AddTurnDataToJson(simulationDataDoc, turnsArray, gameInfo.turnInfo);

    simulationDataDoc.AddMember("seed", gameInfo.turnInfo.front().playedCards.first.front().matchDataBefore.seed, simulationDataDoc.GetAllocator());
    
    const int totalMonsterAmount  = gameInfo.turnInfo.front().playedCards.first.front().matchDataAfter.monsterCards.size();
    int monstersLeft = 0;
    if(!gameInfo.turnInfo.back().playedCards.first.front().matchDataAfter.monsterCards.empty())
    {
        monstersLeft = gameInfo.turnInfo.back().playedCards.first.front().matchDataAfter.monsterCards.size();
    }

    simulationDataDoc.AddMember("monsterskilled", totalMonsterAmount - monstersLeft, simulationDataDoc.GetAllocator());
    simulationDataDoc.AddMember("turns", turnsArray, simulationDataDoc.GetAllocator());

    std::time_t t = std::time(nullptr);

    auto path = Config::Get().GetBalancingData() + "SimulationData-" + ptl::to_string(t) + ".json";
    FILE* fp;
    errno_t err = fopen_s(&fp, path.c_str(), "w+");

    if(err)
    {
        cof::Error("Could not write to \"SimulationData.json\", make sure this file is writable!!!");
        ASSERT_MSG(!err, "Could not load config");;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    simulationDataDoc.Accept(writer);

    const char* output = buffer.GetString();

    fwrite(buffer.GetString(), sizeof(char), buffer.GetLength(), fp);
    fclose(fp);
    cof::Info("Data Of Simulated Game Is Stored!");
}

void tbsg::AISystem::AddTurnDataToJson(rapidjson::Document& document, rapidjson::Value& turnsArray,
    const ptl::vector<TurnInfo>& turnInfo)
{
    int turnCounter = 0;
    
    for (auto& turn : turnInfo)
    {
        ++turnCounter;

        rapidjson::Value turnObject;
        turnObject.SetObject();
        turnObject.MemberReserve(2, document.GetAllocator());

        turnObject.AddMember("turn", turnCounter, document.GetAllocator());

        for(auto& currentCard : turn.playedCards.first)
        {
            if(currentCard.isCardPlayed)
            {
                turnObject.AddMember("encounterid", turnInfo.front().playedCards.first.front().matchDataBefore.monsterCards.size() - 
                                      currentCard.matchDataAfter.monsterCards.size() + 1, document.GetAllocator());

                break;
            }
        }

        AddMatchDataToJson(document, turnObject, turn.matchDataAfter, "matchdataaftercard");
        AddPlayerDataToJson(document, turnObject, turn.playerChanges[0], "player1changes");
        AddPlayerDataToJson(document, turnObject, turn.playerChanges[1], "player2changes");

        rapidjson::Value playerOneBestCards;
        playerOneBestCards.SetArray();
        playerOneBestCards.Reserve(static_cast<rapidjson::SizeType>(turn.playedCards.first.size()), document.GetAllocator());

        for (auto& currentCardInfo : turn.playedCards.first)
        {
            
            rapidjson::Value cardObject;
            cardObject.SetObject();
            cardObject.MemberReserve(4, document.GetAllocator());

            
            cardObject.AddMember("cardid", currentCardInfo.selectedCard.id, document.GetAllocator());
            cardObject.AddMember("cardname", rapidjson::StringRef(currentCardInfo.selectedCard.meta.name.c_str()), document.GetAllocator());
            cardObject.AddMember("utilityscore", currentCardInfo.utilityScore, document.GetAllocator());
            cardObject.AddMember("iscardplayed", currentCardInfo.isCardPlayed, document.GetAllocator());

            AddPlayerDataToJson(document, cardObject, currentCardInfo.playerChanges[0]);

            AddMatchDataToJson(document, cardObject, currentCardInfo.matchDataBefore, 0, "matchdatabefore");
            AddMatchDataToJson(document, cardObject, currentCardInfo.matchDataAfter, 0, "matchdataafter");

            playerOneBestCards.PushBack(cardObject, document.GetAllocator());
        }

        turnObject.AddMember("player1bestcards", playerOneBestCards, document.GetAllocator());

        rapidjson::Value playerTwoBestCards;
        playerTwoBestCards.SetArray();
        playerTwoBestCards.Reserve(static_cast<rapidjson::SizeType>(turn.playedCards.first.size()), document.GetAllocator());

        for (auto& currentCardInfo : turn.playedCards.second)
        {
            rapidjson::Value cardObject;
            cardObject.SetObject();
            cardObject.MemberReserve(6, document.GetAllocator());

            cardObject.AddMember("cardid", currentCardInfo.selectedCard.id, document.GetAllocator());
            cardObject.AddMember("cardname", rapidjson::StringRef(currentCardInfo.selectedCard.meta.name.c_str()), document.GetAllocator());
            cardObject.AddMember("utilityscore", currentCardInfo.utilityScore, document.GetAllocator());
            cardObject.AddMember("iscardplayed", currentCardInfo.isCardPlayed, document.GetAllocator());

            AddPlayerDataToJson(document, cardObject, currentCardInfo.playerChanges[1]);

            AddMatchDataToJson(document, cardObject, currentCardInfo.matchDataBefore, 1, "matchdatabefore");
            AddMatchDataToJson(document, cardObject, currentCardInfo.matchDataAfter, 1, "matchdataafter");

            playerTwoBestCards.PushBack(cardObject, document.GetAllocator());
        }
        turnObject.AddMember("player2bestcards", playerTwoBestCards, document.GetAllocator());

        turnsArray.PushBack(turnObject, document.GetAllocator());
    }
}

void tbsg::AISystem::AddMatchDataToJson(rapidjson::Document& document, rapidjson::Value& cardObject, const MatchData& matchData, ptl::string matchObjectName)
{
    rapidjson::Value matchObject;
    matchObject.SetObject();
    matchObject.MemberReserve(4, document.GetAllocator());

    rapidjson::Value objectName(rapidjson::kStringType);
    objectName.SetString(matchObjectName.c_str(), strlen(matchObjectName.c_str()), document.GetAllocator());

    AddHeroDataToJson(document, matchObject, matchData, 0, "player1");
    AddHeroDataToJson(document, matchObject, matchData, 1, "player2");

    rapidjson::Value monsterData;
    monsterData.SetObject();
    monsterData.MemberReserve(5, document.GetAllocator());

    if(!matchData.monsterCards.empty())
    {
        monsterData.AddMember("monsteralive", true, document.GetAllocator());
        monsterData.AddMember("monsterID", matchData.monsterCards.front().id, document.GetAllocator());
        monsterData.AddMember("monstername", rapidjson::StringRef(matchData.monsterCards.front().meta.name.c_str()), document.GetAllocator());
        monsterData.AddMember("monsterhealth", matchData.monsterCards.front().data.health, document.GetAllocator());
        monsterData.AddMember("monsterarmor", matchData.monsterCards.front().data.armor, document.GetAllocator());
    }
    else
    {
        monsterData.AddMember("MonsterAlive", false, document.GetAllocator());
    }

    matchObject.AddMember("monster", monsterData, document.GetAllocator());

    cardObject.AddMember(objectName, matchObject, document.GetAllocator());
}

void tbsg::AISystem::AddMatchDataToJson(rapidjson::Document& document, rapidjson::Value& cardObject, const MatchData& matchData, int playerIndex, ptl::string matchObjectName)
{
    rapidjson::Value matchObject;
    matchObject.SetObject();
    matchObject.MemberReserve(2, document.GetAllocator());
    
    rapidjson::Value objectName(rapidjson::kStringType);
    objectName.SetString(matchObjectName.c_str(), strlen(matchObjectName.c_str()), document.GetAllocator());

    AddHeroDataToJson(document, matchObject, matchData, playerIndex, "player");
    AddHeroDataToJson(document, matchObject, matchData, !playerIndex, "opponent");

    cardObject.AddMember(objectName, matchObject, document.GetAllocator());
}

void tbsg::AISystem::AddHeroDataToJson(rapidjson::Document& document, rapidjson::Value& matchObject, const MatchData& matchData, int playerIndex, ptl::string playerObjectName)
{
    
    rapidjson::Value playerObject;
    playerObject.SetObject();
    playerObject.MemberReserve(9, document.GetAllocator());
    
    playerObject.AddMember("health",        matchData.heroes[playerIndex].health,           document.GetAllocator());
    playerObject.AddMember("armor",         matchData.heroes[playerIndex].armor,            document.GetAllocator());
    playerObject.AddMember("decksize",      matchData.playerDecks[playerIndex].size(),      document.GetAllocator());
    playerObject.AddMember("discardsize",   matchData.playerDiscards[playerIndex].size(),   document.GetAllocator());
    playerObject.AddMember("handsize",      matchData.playerHands[playerIndex].size(),      document.GetAllocator());

    rapidjson::Value objectName(rapidjson::kStringType);
    objectName.SetString(playerObjectName.c_str(), strlen(playerObjectName.c_str()), document.GetAllocator());

    matchObject.AddMember(objectName, playerObject, document.GetAllocator());
}

void tbsg::AISystem::AddPlayerDataToJson(rapidjson::Document& document, rapidjson::Value& playerArray, const PlayerChanges& playerChanges)
{
    rapidjson::Value playerObject;
    playerObject.SetObject();
    playerObject.MemberReserve(4, document.GetAllocator());

    playerObject.AddMember("healthgained",  playerChanges.healingDone,  document.GetAllocator());
    playerObject.AddMember("healthlost",    playerChanges.damageTaken,  document.GetAllocator());
    playerObject.AddMember("armorgained",   playerChanges.armorGained,  document.GetAllocator());
    playerObject.AddMember("armorlost",     playerChanges.armorLost,    document.GetAllocator());

    playerArray.AddMember("playerchanges", playerObject, document.GetAllocator());
}

void tbsg::AISystem::AddPlayerDataToJson(rapidjson::Document& document, rapidjson::Value& playerArray, const PlayerChanges& playerChanges, ptl::string objectName)
{
    rapidjson::Value playerObject;
    playerObject.SetObject();
    playerObject.MemberReserve(4, document.GetAllocator());

    playerObject.AddMember("healthgained", playerChanges.healingDone, document.GetAllocator());
    playerObject.AddMember("healthlost", playerChanges.damageTaken, document.GetAllocator());
    playerObject.AddMember("armorgained", playerChanges.armorGained, document.GetAllocator());
    playerObject.AddMember("armorlost", playerChanges.armorLost, document.GetAllocator());

    rapidjson::Value memberName(rapidjson::kStringType);
    memberName.SetString(objectName.c_str(), strlen(objectName.c_str()), document.GetAllocator());

    playerArray.AddMember(memberName, playerObject, document.GetAllocator());
}

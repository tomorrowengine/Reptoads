#pragma once
#include "memory/Containers.h"
#include "Payloads.h"
#include "gameplay/DataStructs.h"
#include <future>
#include <rapidjson/document.h>

namespace tbsg
{
    struct Hero;
    class GameDataDatabase;
    
	/*! \addtogroup Gameplay
	*  Our AI System
	*  @{
	*/

	enum class UtilityFunctions : int
	{
		Player_Health = 0,
		Opponent_Health = 1,
		Monster_Health = 2,
		Player_Armor = 5,
		Opponent_Armor = 6,
		Monster_Armor = 7,
		Player_Hand = 10,
		Opponent_Hand = 11,
		Player_Deck = 12,
		Opponent_Deck = 13

	};

	// This is a struct to keep track of the changes that happened to the stats or the players
    struct PlayerChanges
    {
        unsigned int healingDone    = 0;
        unsigned int damageTaken    = 0;
        unsigned int armorGained    = 0;
        unsigned int armorLost      = 0;
    };

	// This is a struct to keep track of data about the card that is played, what the game status was before it was played and after it was played.
    struct PlayedCard
    {
        Card selectedCard;
        MatchData matchDataBefore;
        MatchData matchDataAfter;
        ptl::vector<PlayerChanges> playerChanges{ 2 };
        bool isCardPlayed   = false;
        float utilityScore  = 0;
        
    };

	// This struct contains every piece of information that we want to keep track of in a turn
    struct TurnInfo
    {
        MatchData matchDataAfter;
        std::pair<ptl::vector<PlayedCard>, ptl::vector<PlayedCard>> playedCards;
        ptl::vector<PlayerChanges> playerChanges{ 2 };
    };

	// This struct contains all the data that we want to get from a game
    struct GameInfo
    {
        ptl::vector<TurnInfo> turnInfo;
    };

    class AISystem
    {
    public:
        AISystem() = default;
        ~AISystem() = default;

		/**
		 * \brief This is used to initialize the AI and set the GameDataModel reference and the amount of sets of cards to store
		 * \param gameDataModel
		 * \param amountOfPlaysToStore
		 */
        void Initialize(GameDataDatabase* gameDataModel, size_t amountOfPlaysToStore = 3);

		/**
		 * \brief This function is used to run game simulations and generate balancing information
		 */
        void RunBalancingTool(MatchData& matchData);

		/**
		 * \brief This function is used to make the AI select a card using the given match data and player ID. This is meant for using the AI as an opponent.
		 * \param matchData
		 * \param aiPlayerID
		 * \return Card ID
		 */
        unsigned int RunGameAI(MatchData& matchData, unsigned int aiPlayerID);

		/**
		 * \brief This function is used to Simulate a game with the given data, it makes two AIs play against eachother while collecting data that can be used for balancing
		 * \param simulationMatchData
		 * \return It returns the cards played and the utility scores attached to these cards
		 */
        void SimulateGame(MatchData& simulationMatchData);


    public:

    private:
        
		/**
		 * \brief This function picks the 4 cards to place on the slots
		 * \param simulationMatchData
		 * \param playerID
		 * \return ptl::vector<PlayedCard>
		 */
        ptl::vector<tbsg::PlayedCard> PickCards(MatchData& simulationMatchData, unsigned int playerID);

		/**
		* \brief This funciton runs a simulation of a turn and returns the card that was played in the end
		* \param simulationMatch
		* \param playerID
		* \param selectedCard
		* \return PlayedCard
		*/
        PlayedCard SimulateTurn(MatchData& simulationMatch, unsigned int playerID, unsigned int selectedCard);

		/**
		* \brief This function is used get the best cards (multiple) that the player can play based on the utility scores, this 
		* \param topCards
		* \return float
		*/
        ptl::vector<PlayedCard> GetTopCards(ptl::vector<PlayedCard>& playedCards, MatchData& simulationMatchData);

		/**
		 * \brief This function is used get the best card that the player can play based on the utility scores
		 * \param topCards
		 * \return float
		 */
        unsigned int GetBestCard(ptl::vector<PlayedCard>& topCards);

		/**
		 * \brief This function loads all the utility formulas from the Json file and stores the lambdas in the vector
		 */
        void LoadUtilityFormulas();

		/**
		 * \brief This function is used to calculate the utility scores of a played card
		 * \param matchDummy
		 * \param originalMatchData
		 * \param playerID
		 * \return float
		 */
        float CalculateUtility(MatchData& matchDummy, MatchData& originalMatchData, unsigned int playerID);

		/**
		 * \brief This function is used to add multiple scores and get the average score
		 * \param utilityScores
		 * \return float
		 */
        float AddUtilityScores(ptl::vector<float>& utilityScores);

		/**
		 * \brief This function is a utility function used to print hte deck of a player to the console
		 * \param deck
		 * \param deckType
		 * \return float
		 */
        void PrintDeck(const ptl::vector<unsigned int>& deck, const ptl::string& deckType);

		/**
		 * \brief This function gathers the data of what happened in a turn and adds it to the playerChanges
		 * \param deck
		 * \param deckType
		 * \return float
		 */
        void GetChangeData(const ResultOfRound& results, ptl::vector<PlayerChanges>& playerChanges);

		// These functions are used to add the gathered data to a Json file
        void StoreGatheredGameData(const GameInfo& gameInfo);

        void AddTurnDataToJson(rapidjson::Document& document, rapidjson::Value& turnsArray, const ptl::vector<TurnInfo>& turnInfo);
        void AddMatchDataToJson(rapidjson::Document& document, rapidjson::Value& cardObject, const MatchData& matchData, ptl::string matchObjectName);
        void AddMatchDataToJson(rapidjson::Document& document, rapidjson::Value& cardObject, const MatchData& matchData, int playerIndex, ptl::string matchObjectName);
        void AddHeroDataToJson(rapidjson::Document& document, rapidjson::Value& matchObject, const MatchData& matchData, int playerIndex, ptl::string playerObjectName);
        void AddPlayerDataToJson(rapidjson::Document& document, rapidjson::Value& playerArray, const PlayerChanges& playerChanges);
        void AddPlayerDataToJson(rapidjson::Document& document, rapidjson::Value& playerArray, const PlayerChanges& playerChanges, ptl::string objectName);

    private:
        tbsg::GameDataDatabase* gameDataModelRef = nullptr;

        size_t numberOfPlaysToStore = 3;

		// Once the utility functions get loaded, the lambdas will be stored in here so that we can easily access them later.
        std::vector<std::pair<std::function<float(float)>, float>> utilityFunctions;

		// This sets a limit to the amout of turns that should be played
        bool limitTurnAmount = true;

    };
	/*! @} */
}

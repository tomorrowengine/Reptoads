#pragma once
#include "DataStructs.h"

namespace tbsg
{
    class GameDataDatabase;
    namespace  scripting
    {
        class LuaContext;
    }

	namespace gameplay
	{
		tbsg::MatchData CreateMatchData(GameDataDatabase* gameDataModel, ptl::string monsterDeck, ptl::string playerOneDeck, 
			ptl::string playerTwoDeck, unsigned int playerOneID = 0, unsigned int playerTwoID = 0, unsigned int seed = 0);

		tbsg::MatchData CreateMatchData(ptl::vector<tbsg::MonsterCard>& monsterCards,
			ptl::vector<tbsg::Deck>& decksToUse, ptl::vector<ptl::vector<Card*>> cardsInHand,
			ptl::vector<tbsg::Hero> heroObjsToUse, unsigned int playerOneID = 0, unsigned int playerTwoID = 0,
			unsigned int playerCount = 2, unsigned int playerNumberThatStarts = 1, unsigned int seed = 0);

		tbsg::MatchData CreateMatchData(ptl::vector<tbsg::MonsterCard>& monsterCards,
			tbsg::Deck& player1Deck, tbsg::Deck& player2Deck, ptl::vector<ptl::vector<Card*>> cardsInHand,
			ptl::vector<tbsg::Hero> heroObjsToUse, unsigned int playerOneID = 0, unsigned int playerTwoID = 0,
			unsigned int playerCount = 2, unsigned int playerNumberThatStarts = 1, unsigned int seed = 0);

		tbsg::MatchData CreateMatchData(tbsg::MonsterDeck& monsterDeck,
			ptl::vector<tbsg::Deck>& decksToUse, ptl::vector<ptl::vector<Card*>> cardsInHand,
			ptl::vector<tbsg::Hero> heroObjsToUse, unsigned int playerOneID = 0, unsigned int playerTwoID = 0, 
			unsigned int playerCount = 2, unsigned int playerNumberThatStarts = 1, unsigned int seed = 0);

		

		
		bool CreateContainer(ptl::vector<ptl::vector<unsigned int>>& toAddTowards, unsigned int amountToAdd = 0, unsigned int addUntilLimit = 0);
		bool CreateContainer(ptl::vector<tbsg::Hero>& toAddTowards, unsigned int amountToAdd = 0, unsigned int addUntilLimit = 0);

		bool FillContainer(ptl::vector<unsigned int>& toAddTowards, unsigned int amountToAdd = 0, unsigned int addUntilLimit = 0);
		bool FillContainer(ptl::vector<tbsg::Card*>& toAddTowards, unsigned int amountToAdd = 0, unsigned int addUntilLimit = 0);
		bool FillContainer(ptl::vector<tbsg::Deck>& toAddTowards, unsigned int amountToAdd = 0, unsigned int addUntilLimit = 0);

		ptl::vector<unsigned int> ConvertDeckToCardNumbers(const tbsg::Deck& toCreateFrom);
		ptl::vector<unsigned int> ConvertCardListToCardIDs(const ptl::vector<tbsg::Card*>& toCreateFrom);

		
		ptl::vector<unsigned int> CreatePlaceHolderList(const unsigned int amountToFill);
		ptl::vector<unsigned int> CreateCardListFrom(const ptl::vector<unsigned int>& createFromExistingIDS);
		ptl::vector<unsigned int> CreateCardListFrom(const ptl::vector<tbsg::Card*>& createFromExistingCards);
		ptl::vector<unsigned int> CreateCardListFrom(const tbsg::Deck& createFromExistingDeck);

		
		ptl::vector<tbsg::Card*> CreateCardListFromDeck(const tbsg::Deck& createFromExistingDeck);

		void PlaceCardListTo(ptl::vector<unsigned int>& toPlaceIn, const ptl::vector<unsigned int> toCopyFrom);
		void PlaceCardListTo(ptl::vector<unsigned int>& toPlaceIn, const ptl::vector<tbsg::Card*>& toCopyFrom);
		void PlaceCardListTo(ptl::vector<unsigned int>& toPlaceIn, const tbsg::Deck& toCopyFrom);


        tbsg::ResultOfRound OnPlay(tbsg::MatchData& matchData, scripting::LuaContext& luaContext, GameDataDatabase* gameDataModel);

        void GiveReward(Hero& a_Hero, ptl::vector<Reward*>& a_Reward, tbsg::ResultOfRound& a_ResultOfRound, int a_PlayerID);
        void BaseCardEffect(MatchData& matchData, int currentPlayerID, ptl::vector<BaseCardEffects>& baseCardEffects, tbsg::ResultOfRound& resultOfRound, bool isAtEndFight);
        void ResetHeroStats(tbsg::Hero& hero);
        bool HandleDeath(ptl::vector<bool> isHeroDead, int playerCount, MatchData& matchData);
	}
}

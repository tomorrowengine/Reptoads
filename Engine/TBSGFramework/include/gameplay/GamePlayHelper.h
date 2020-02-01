#pragma once
#include "memory/Containers.h"
#include "Payloads.h"
#include "sol.hpp"
#include "gameplay/scripting/LuaContext.h"




namespace tbsg
{
	namespace scripting
	{
		class LuaContext;
	}

	namespace gameplay
	{

		void ShuffleCards(ptl::vector<unsigned int>& cardsToShuffle);
		void ShuffleCards(ptl::vector<tbsg::Card*>& cardsToShuffle);

		void ReverseCardOrder(ptl::vector<unsigned int>& cardsToReverse);
		void ReverseCardOrder(ptl::vector<tbsg::Card*>& cardsToReverse);

		void DamageToHero(unsigned int damage, Hero& receiver, unsigned int receiverID, ResultOfRound& resultOfRound);
		void DamageToCard(unsigned int damage, MonsterCard& receiver, ResultOfRound& resultOfRound);

		void CardAddHealth(int health, MonsterCard& receiver, ResultOfRound& resultOfRound);
		void CardAddArmor(int armor, MonsterCard& receiver, ResultOfRound& resultOfRound);

		void HeroAddHealth(int health, Hero& receiver, unsigned int receiverID, ResultOfRound& resultOfRound);
		void HeroAddArmor(int armor, Hero& receiver, unsigned int receiverID, ResultOfRound& resultOfRound);

		int ProssesChange(int change, unsigned int & amountToChange, int min, int max);

		ptl::vector<unsigned int> DrawCards(ptl::vector<unsigned int>& toDrawFrom, ptl::vector<unsigned int>& toPlaceIn, unsigned int amountToDraw = 0, unsigned int drawUntilLimit = 0);
		ptl::vector<unsigned int> DrawCards(ptl::vector<tbsg::Card*>& toDrawFrom, ptl::vector<tbsg::Card*>& toPlaceIn, unsigned int amountToDraw = 0, unsigned int drawUntilLimit = 0);

		void RemoveCard(ptl::vector<unsigned int>& toRemoveFrom, const unsigned int whatToRemove);
		void RemoveCard(ptl::vector<tbsg::Card*>& toRemoveFrom, const tbsg::Card* whatToRemove);

		bool RemoveCards(ptl::vector<unsigned int>& toRemoveFrom, ptl::vector<unsigned int>& whatToRemove);

		bool ValueInsideContainer(const ptl::vector<unsigned int>& toCheckFrom, const unsigned int valueToCheck);

		unsigned int GetRandomIntInRange(const unsigned int minNumber, const unsigned int maxNumber, std::mt19937& seed);

		
		bool RunLuaScript(tbsg::scripting::LuaContext& luaContextToUse, const tbsg::Script& scriptToRun);

		bool CallFunctionFromLua(tbsg::scripting::LuaContext& luaContextToUse, const tbsg::Script& scriptToRun, const ptl::string functionNameToCall, tbsg::Card* cardObjToUse = nullptr);
		template<typename ...Args>
		bool CallFunctionFromLua(tbsg::scripting::LuaContext& luaContextToUse, const tbsg::Script& scriptToRun, const ptl::string functionNameToCall, Args...args)
		{
			if (!RunLuaScript(luaContextToUse, scriptToRun)) { return false; }
			else
			{
				sol::protected_function func;
				luaContextToUse.GetFunction(func, functionNameToCall.c_str());
				sol::protected_function_result result;
				result = func.call(args...);

				if (!func.valid() && luaContextToUse.GetCallStatus(result) != sol::call_status::ok) {
					std::cout << "[Sol2] ERROR running Lua Script with ID: " << scriptToRun.id << " with Function Call: " << functionNameToCall << " wasn't valid!" << std::endl;
					return false;
				}
			}
			return true;
		}

	}
}

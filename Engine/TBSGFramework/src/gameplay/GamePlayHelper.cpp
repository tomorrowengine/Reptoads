#include <iostream>
#include <random>
#include <algorithm>
#include "gameplay/GamePlayHelper.h"



bool tbsg::gameplay::RunLuaScript(tbsg::scripting::LuaContext& luaContextToUse, const tbsg::Script& scriptToRun)
{
	sol::safe_function_result scriptCode = luaContextToUse.RunScript(scriptToRun.code);
	if (!luaContextToUse.IsValid(scriptCode))
	{
		std::cout << "[Sol2] ERROR running Lua Script with ID: " << scriptToRun.id << std::endl << " Error message: " <<
			luaContextToUse.GetErrorMessage(scriptCode) << " Lua Script: " << scriptToRun.code << std::endl;
		return false;
	}
	return true;
}

bool tbsg::gameplay::CallFunctionFromLua(tbsg::scripting::LuaContext& luaContextToUse, const tbsg::Script& scriptToRun, const ptl::string functionNameToCall, tbsg::Card* cardObjToUse)
{
	if (!RunLuaScript(luaContextToUse, scriptToRun)) { return false; }
	else
	{
		sol::protected_function func;
		luaContextToUse.GetFunction(func, functionNameToCall.c_str());
		sol::protected_function_result result;
		if (cardObjToUse != nullptr)
		{
			result = func.call(cardObjToUse);
		}
		else
		{
			result = func.call();
		}

		if (!func.valid() && luaContextToUse.GetCallStatus(result) != sol::call_status::ok) {
			sol::error err = result;
			std::string what = err.what();
			std::cout << "[Sol2] ERROR running Lua Script with ID: " << scriptToRun.id << " with Function Call: " << functionNameToCall << " wasn't valid!" << std::endl;
			return false;
		}
		else if (!func.valid()) {
			sol::error err = result;
			std::string what = err.what();
			std::cout << "[Sol2] ERROR running Lua Script with ID: " << scriptToRun.id << " with Function Call: " << functionNameToCall << " wasn't valid!" << std::endl;
			std::cout << "[Sol2] ERROR: Runtime:" << what << "\n";
			return false;
		}
	}
	return true;
}


void tbsg::gameplay::ShuffleCards(ptl::vector<unsigned int>& cardsToShuffle)
{
	std::random_device rd;
	std::mt19937 randomSeed(rd());

	std::shuffle(cardsToShuffle.begin(), cardsToShuffle.end(), randomSeed);
}

void tbsg::gameplay::ShuffleCards(ptl::vector<tbsg::Card*>& cardsToShuffle)
{
	std::random_device rd;
	std::mt19937 randomSeed(rd());

	std::shuffle(cardsToShuffle.begin(), cardsToShuffle.end(), randomSeed);
}

void tbsg::gameplay::ReverseCardOrder(ptl::vector<unsigned int>& cardsToReverse)
{
	std::reverse(cardsToReverse.begin(), cardsToReverse.end());
}

void tbsg::gameplay::ReverseCardOrder(ptl::vector<tbsg::Card*>& cardsToReverse)
{
	std::reverse(cardsToReverse.begin(), cardsToReverse.end());
}

void tbsg::gameplay::DamageToHero(unsigned int damage, Hero & receiver, unsigned int receiverID, ResultOfRound& resultOfRound)
{
	if (receiver.armor > 0)
	{
		Change changeArmor;
		int newArmor = receiver.armor - damage;
		if (newArmor < 0)
		{
			damage = -newArmor;
			changeArmor.change = receiver.armor * -1;
			receiver.armor = 0;
		}
		else
		{
			changeArmor.change = damage * -1;
			receiver.armor -= damage;
			damage = 0;
		}
		changeArmor.index = receiverID;
		changeArmor.changeType = EffectChange::Hero_Armor;

		if (changeArmor.change != 0)
		{
			resultOfRound.results.push_back(changeArmor);
		}
	}

	if (receiver.health > 0 && damage > 0)
	{
		Change changeHealth;
		int newHealth = receiver.health - damage;
		if (newHealth < 0)
		{
			changeHealth.change = receiver.health * -1;
			receiver.health = 0;
		}
		else
		{
			changeHealth.change = damage * -1;
			receiver.health -= damage;
		}
		changeHealth.index = receiverID;
		changeHealth.changeType = EffectChange::Hero_Health;

		if (changeHealth.change != 0)
		{
			resultOfRound.results.push_back(changeHealth);
		}
	}
}

void tbsg::gameplay::DamageToCard(unsigned int damage, MonsterCard & receiver, ResultOfRound& resultOfRound)
{
	if (receiver.data.armor > 0)
	{
		Change changeArmor;
		int newArmor = receiver.data.armor - damage;
		if (newArmor < 0)
		{
			damage = receiver.data.armor;
			changeArmor.change = receiver.data.armor * -1;
			receiver.data.armor = 0;
		}
		else
		{
			changeArmor.change = damage * -1;
			receiver.data.armor -= damage;
			damage = 0;
		}
		changeArmor.index = 0;
		changeArmor.changeType = EffectChange::Card_Armor;

		if (changeArmor.change != 0)
		{
			resultOfRound.results.push_back(changeArmor);
		}
	}

	if (receiver.data.health > 0 && damage > 0)
	{
		Change changeHealth;
		int newHealth = receiver.data.health - damage;
		if (newHealth < 0)
		{
			changeHealth.change = receiver.data.health * -1;
			receiver.data.health = 0;
		}
		else
		{
			changeHealth.change = damage * -1;
			receiver.data.health -= damage;
		}
		changeHealth.index = 0;
		changeHealth.changeType = EffectChange::Card_Health;

		if (changeHealth.change != 0)
		{
			resultOfRound.results.push_back(changeHealth);
		}
	}
}

void tbsg::gameplay::CardAddHealth(int health, MonsterCard & receiver, ResultOfRound & resultOfRound)
{
	Change change;
	change.changeType = EffectChange::Card_Health;
	change.index = 0;
	change.change = ProssesChange(health, receiver.data.health, 0, receiver.data.maxHealth);

	if (change.change != 0)
	{
		resultOfRound.results.push_back(change);
	}
}

void tbsg::gameplay::CardAddArmor(int armor, MonsterCard & receiver, ResultOfRound & resultOfRound)
{
	Change change;
	change.changeType = EffectChange::Card_Armor;
	change.index = 0;
	change.change = ProssesChange(armor, receiver.data.armor, 0, 123456789);

	if (change.change != 0)
	{
		resultOfRound.results.push_back(change);
	}
}

void tbsg::gameplay::HeroAddHealth(int health, Hero & receiver, unsigned int receiverID, ResultOfRound & resultOfRound)
{
	UNUSED(receiverID);
	Change change;
	change.changeType = EffectChange::Hero_Health;
	change.index = receiverID;
	change.change = ProssesChange(health, receiver.health, 0, receiver.maxHealth);

	if (change.change != 0)
	{
		resultOfRound.results.push_back(change);
	}
}

void tbsg::gameplay::HeroAddArmor(int armor, Hero & receiver, unsigned int receiverID, ResultOfRound & resultOfRound)
{
	UNUSED(receiverID);
	Change change;
	change.changeType = EffectChange::Hero_Armor;
	change.index = receiverID;
	change.change = ProssesChange(armor, receiver.armor, 0, 123456789);

	if (change.change != 0)
	{
		resultOfRound.results.push_back(change);
	}
}

int tbsg::gameplay::ProssesChange(int change, unsigned int & amountToChange, int min, int max)
{
	int newChange = amountToChange + change;
	int changeAmount = 0;

	if (newChange < min)
	{
		changeAmount = amountToChange * -1;
		amountToChange = 0;
	}
	else if (newChange > max)
	{
		changeAmount = max - amountToChange;
		amountToChange = max;
	}
	else
	{
		changeAmount = change;
		amountToChange += change;
	}

	return changeAmount;
}

ptl::vector<unsigned int> tbsg::gameplay::DrawCards(ptl::vector<unsigned int>& toDrawFrom, ptl::vector<unsigned int>& toPlaceIn, unsigned int amountToDraw, unsigned int drawUntilLimit)
{
	ptl::vector<unsigned int> cardsDrawn{};

	if (amountToDraw == 0 || toDrawFrom.empty()) {
		return cardsDrawn;
	}
	else
	{
		if (drawUntilLimit != 0)
		{
			
			if ((toPlaceIn.size() + amountToDraw) > drawUntilLimit) {
				amountToDraw = drawUntilLimit - static_cast<std::uint32_t>(toPlaceIn.size());
			}
		}

		
		if (toDrawFrom.size() < amountToDraw) {
			amountToDraw = static_cast<std::uint32_t>(toDrawFrom.size());
		}
		
		for (unsigned int i = 0; i < amountToDraw; ++i)
		{
			cardsDrawn.emplace_back(toDrawFrom[i]);
			toPlaceIn.emplace_back(toDrawFrom[i]);
		}
		toDrawFrom.erase(toDrawFrom.begin(), toDrawFrom.begin() + amountToDraw);
	}
	return cardsDrawn;
}

ptl::vector<unsigned int> tbsg::gameplay::DrawCards(ptl::vector<tbsg::Card*>& toDrawFrom, ptl::vector<tbsg::Card*>& toPlaceIn, unsigned int amountToDraw , unsigned int drawUntilLimit )
{
	ptl::vector<unsigned int> cardsDrawn{};
	if (amountToDraw == 0) {
		return cardsDrawn;
	}
	else
	{
		
		if ((toPlaceIn.size() + amountToDraw) > drawUntilLimit) {
			amountToDraw = drawUntilLimit - static_cast<std::uint32_t>(toPlaceIn.size());
		}

		
		if (toDrawFrom.size() < amountToDraw) {
			amountToDraw = static_cast<std::uint32_t>(toDrawFrom.size());
		}

		
		for (unsigned int i = 0; i < amountToDraw; ++i)
		{
			cardsDrawn.push_back(toDrawFrom[i]->id);
			toPlaceIn.push_back(std::move(toDrawFrom[i]));
		}
		toDrawFrom.erase(toDrawFrom.begin(), toDrawFrom.begin() + amountToDraw);
	}
	return cardsDrawn;
}

void tbsg::gameplay::RemoveCard(ptl::vector<unsigned int>& toRemoveFrom, const unsigned int whatToRemove)
{
	auto itr = std::find_if(toRemoveFrom.begin(), toRemoveFrom.end(), [&whatToRemove](unsigned int id)
		{
			return id == whatToRemove;
		});
	if (itr != toRemoveFrom.end()) {
		toRemoveFrom.erase(itr);
	}
}

void tbsg::gameplay::RemoveCard(ptl::vector<tbsg::Card*>& toRemoveFrom, const tbsg::Card* whatToRemove)
{
	toRemoveFrom.erase(
		std::remove(toRemoveFrom.begin(), toRemoveFrom.end(), whatToRemove), toRemoveFrom.end()
	);
}

bool tbsg::gameplay::RemoveCards(ptl::vector<unsigned int>& toRemoveFrom, ptl::vector<unsigned int>& whatToRemove)
{
	toRemoveFrom.erase(
		std::remove_if(
			toRemoveFrom.begin(), toRemoveFrom.end(),
			[&whatToRemove](unsigned int i)
			{
				return ValueInsideContainer(whatToRemove, i);
			}), toRemoveFrom.end()
				);
	return true;
}


bool tbsg::gameplay::ValueInsideContainer(const ptl::vector<unsigned int>& toCheckFrom, const unsigned int valueToCheck)
{
	auto result = std::find(std::begin(toCheckFrom), std::end(toCheckFrom), valueToCheck);
	if (result != std::end(toCheckFrom))
	{
		return true;
	}
	return false;
}


unsigned int tbsg::gameplay::GetRandomIntInRange(const unsigned int minNumber, const unsigned int maxNumber, std::mt19937& seed)
{
	std::uniform_int_distribution<> range(minNumber, maxNumber);

	return range(seed);
}

#pragma once

#include "memory/string.h"
#include "memory/Containers.h"

namespace tbsg
{

	

	 
	struct Unit;

	
	enum class CardRewardType
	{
		None = 0,
		Weapon,
		Attack,
		Resource,
		Armor,
		Health
	};

	
	enum class StatType
	{
		None = 0,
		Health,
		Attack,
		Armor,
		Resource,
		WeaponAttack,
		WeaponDurability
	};

	enum class EffectChange
	{
		None = 0,

		Card_Health,
		Card_Attack,
		Card_Armor,
		Card_Particle,
		Card_Animation,
        Card_Death,

		Hero_Weapon_Attack,
		Hero_Weapon_Durability,
		Hero_Health,
		Hero_Resource,
		Hero_Armor,
		Hero_Attack,
		Hero_Particle,
		Hero_Animation,
		Hero_Death,
        Hero_WinLose,

		Reward_Attack,
		Reward_Health,
		Reward_Armor,
		Reward_Resource,
		Reward_Weapon_Attack,
		Reward_Weapon_Durability,

		Deck_Shuffle,
		Deck_DiscardToDeck,
		Deck_AddCard,
		Deck_RemoveCard,
		Deck_DrawCard,

		Hand_AddCard,
		Hand_RemoveCard,

		Discard_AddCard,
		Discard_RemoveCard,

		NextMonster,
		NextStartingPlayer,
		Play_Sound
	};

	enum class BaseEffect
	{
		None = 0,
		MonsterDamage,
		OpponentDamage,
		SelfDamage,

		MonsterHealth,
		OpponentHealth,
		SelfHealth,

		MonsterArmor,
		OpponentArmor,
		SelfArmor,

		OpponentDrawCard,
		SelfDrawCard,

		OpponentDiscardCard,
		SelfDiscardCard
	};


	
	struct MetaData
	{
		ptl::string name{};
		ptl::string description{};
        unsigned int rarity{};
        unsigned int type{};
	};


	
	struct Weapon
	{
		unsigned int id{};
		unsigned int attack{};
		unsigned int durability{};
	};


	
	struct Reward
	{
		CardRewardType type{};
		unsigned int powerup{};
		Weapon* weapon = nullptr;
	};


	struct BaseCardEffects
	{
		BaseEffect baseEffect{};
		int effectValue{};
	};


	
	struct CardData
	{
		ptl::vector<BaseCardEffects> baseCardEffects{};
	};

    struct MonsterData
    {
        unsigned int health{};
        unsigned int maxHealth{};
        unsigned int armor{ 0 };
        unsigned int monsterTrait{};
        ptl::vector<Reward*> reward{};
    };

	
	struct Hero
	{
		unsigned int health{ 40 };
		unsigned int maxHealth{ 40 };
		unsigned int resource{ 0 };
		unsigned int armor{ 0 };
		unsigned int attack{ 1 };
		unsigned int baseAttack{ 1 };
		Weapon* weapon{ nullptr };
	};

	
	struct Script
	{
		unsigned int id{};
		
		unsigned int cardId{ 0 };
		
		unsigned int monsterCardId{ 0 };
		
		ptl::string name{};
		ptl::string code{};
	};

	
	struct Card
	{
		unsigned int id{};
		MetaData meta{};
		CardData data{};
	};
    
    struct MonsterCard
    {
        unsigned int id{};
        MetaData meta{};
        MonsterData data{};
    };

	struct CardRarity
	{
		unsigned int id{};
		ptl::string name{};
	};

	struct CardType
	{
		unsigned int id{};
		ptl::string name{};
	};

	
	struct Deck
	{
		unsigned int id{};
		ptl::string name{};
		ptl::vector<Card*> cards{};
	};

    
    struct MonsterDeck
    {
        unsigned int id{};
        ptl::string name{};
        ptl::vector<MonsterCard*> cards{};
    };

	
	struct Profile
	{
		unsigned int id{};
		ptl::string username{};
		unsigned int connectionId{ 0 }; 
	};

	
	struct Play
	{
		
		unsigned int playerIndex{};
		
		unsigned int playedCard{};
	};

	
	struct Change
	{
		
		EffectChange changeType{};
		
		int change{};
		
		unsigned int index{};
	};

	
	struct ResultOfRound
	{
		ptl::vector<unsigned int> playedCards{ 2 };
		ptl::vector<Change> results{};
	};

	

	struct ClientMatchState
	{
        
        ptl::vector<tbsg::MonsterCard> monsterCards{ };

        
        ptl::vector<tbsg::Hero> heroes{ };

        ptl::vector<ptl::vector<unsigned int>> playerDecks{ 2 };
        ptl::vector<ptl::vector<unsigned int>> playerHands{ 2 };
        ptl::vector<ptl::vector<unsigned int>> playerDiscards{ 2 };
	};

	
	struct Match
	{
		unsigned int id{}; 
		unsigned int ownerProfileId{};
		unsigned int ownerDeckId{};
		unsigned int opponentProfileId{}; 
		unsigned int opponentDeckId{}; 
		unsigned int serverId{};
	};

	struct Server
	{
		unsigned int id{};
		std::string hostname{};
		std::string ip{};
		unsigned int port{};
		bool occupied{};
	};

	struct LobbyData
	{
		unsigned int id{};
		unsigned int ownerConnectionId{};
		ptl::string ownerName{};
		unsigned int opponentConnectionId{};
		ptl::string opponentName{};
		bool inGame{};
	};

    struct EffectEvent
    {
        ptl::string customEventName;
        
        std::function<bool()> eventDelegate;
    };
}

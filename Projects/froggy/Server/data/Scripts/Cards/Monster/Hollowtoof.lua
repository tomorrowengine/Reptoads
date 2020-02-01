require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        local slainGoblinAmount = GetAmountOfCreatureSlainByFamily(GetCurrentPlayerID, GetCurrentCardIndex, "Goblin")
        local damage = 2 + (2 * slainGoblinAmount)
        AddHeroHealth(GetRival(), damage)
    end
end
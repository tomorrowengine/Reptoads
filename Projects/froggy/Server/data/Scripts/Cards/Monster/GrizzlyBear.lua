require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        local nextCreature = GetNextCreature(GetCurrentPlayerID(), GetCurrentCardIndex())
        AddCardHealth(nextCreature, -2)
    end
end
require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        local nextSupport = GetNextSupportByFamily(GetCurrentPlayerID(), GetCardPlayedIndex(), "Equipment")
        AddCardCost(nextSupport, -4)
    end
end
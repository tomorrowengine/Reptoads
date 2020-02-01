require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        local nextSupportCard = GetNextSupport(GetCurrentPlayerID(), GetCardPlayedIndex())
        local cardCost = GetCardCost(nextSupportCard)
        SetCardCost(cardCost * 0.5)
    end
end
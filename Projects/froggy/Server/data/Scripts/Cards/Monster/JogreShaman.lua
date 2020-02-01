require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        playerID = GetCurrentPlayerID()
        rivalHero = GetRival()
        handSize = GetHandSize(playerID)
        DealDamageToHero(rivalHero, handSize + 4)
    end
end
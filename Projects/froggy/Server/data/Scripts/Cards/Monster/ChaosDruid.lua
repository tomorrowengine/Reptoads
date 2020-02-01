require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        AddRandomCardToHandBasedOnFamily(GetCurrentPlayerID(), "Potion")
        AddRandomCardToHandBasedOnFamily(GetCurrentPlayerID(), "Potion")
        AddCardToHand(GetCurrentPlayerID(), "VialofWater")
    end
end
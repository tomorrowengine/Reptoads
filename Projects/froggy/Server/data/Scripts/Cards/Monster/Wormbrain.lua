require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        AddHeroResource(hero,-2)
        playerIndex = GetCurrentPlayerID()
        AddCardToHand(playerIndex, "Elvarg")
    end
end
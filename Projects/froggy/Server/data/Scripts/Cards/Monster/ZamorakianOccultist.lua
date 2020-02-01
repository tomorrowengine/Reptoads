require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        playerIndex = GetCurrentPlayerID()
        AddCardToHand(playerIndex, "Asphyxiate")
        AddCardToHand(playerIndex, "Asphyxiate")
    end
end
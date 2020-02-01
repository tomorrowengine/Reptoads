require('CombatLibrary')
function OnCardPlay(a_Card)
    playerID = GetCurrentPlayerID()

    handSize = GetHandSize(playerID)
    rivalHero = GetRival()
    DealDamageToHero(rivalhero, handSize)
end
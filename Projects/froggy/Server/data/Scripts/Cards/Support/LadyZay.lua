require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    amountOfCards = GetAmountOfCardsInHandByName(playerIndex, "Cannonball")
    
    rivalHero = GetRival()
    DealDamageToHero(rivalHero,amountOfCards * 2)
    RemoveCardsFromHandByName(playerIndex, "Cannonball", amountOfCards)

end
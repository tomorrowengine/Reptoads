require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    amountOfCards = GetAmountOfCardsInHandByType(playerIndex, "Support")

    rivalHero = GetRival()
    DealDamageToHero(rivalHero,amountOfCards * 2)

end
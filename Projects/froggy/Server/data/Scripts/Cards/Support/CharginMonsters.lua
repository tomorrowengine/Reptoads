function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    rivalIndex = GetCurrentRivalID()
    currentCardIndex = GetCardPlayedIndex()
    amountOfCards = GetAmountOfCardsInHandByFamily(playerIndex, "Ogres")

    AddHealthToCreaturesInPlay(playerIndex, amountOfCards * -1)

end
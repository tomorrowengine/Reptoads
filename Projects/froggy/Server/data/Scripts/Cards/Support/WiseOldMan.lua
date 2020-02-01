function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    handSize = GetHandSize(playerID)
    currentCardIndex = GetCardPlayedIndex()
    AddHealthToCreaturesInPlay(playerID, currentCardIndex, handSize)
end
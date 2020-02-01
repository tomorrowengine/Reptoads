function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()

    nextSupport = GetNextSupport(playerID, currentCardIndex)
    AddCardCost(nextSupport, -3)
end
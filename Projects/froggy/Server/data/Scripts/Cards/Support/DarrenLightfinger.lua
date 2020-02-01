function OnCardPlay(a_Card)

    playerID = GetCurrentPlayerID()
    RemoveRandomCardFromHand(playerID)
    
    currentCardPlayedIndex = GetCardPlayedIndex()
    nextSupport = GetNextSupport(playerID, currentCardPlayedIndex)
    AddCardCost(nextSupport, -4)
end
function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    rivalIndex = GetCurrentRivalID()
    currentCardIndex = GetCardPlayedIndex()
   
    playerNextPurchase = GetNextPurchase(playerIndex, currentCardIndex)
    rivalNextPurchase = GetNextPurchase(rivalIndex, currentCardIndex)

    AddCardBaseCost(playerNextPurchase, -3)
    AddCardBaseCost(rivalNextPurchase, -1)
end
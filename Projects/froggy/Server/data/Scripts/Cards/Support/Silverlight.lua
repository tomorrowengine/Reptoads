function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    AddHealthToCreaturesInPlayByFamily(playerID, currentCardIndex, "Demon", -2)
end
function OnCardPlay(a_Card)
    playerID = GetCurrentPlayerID() 
    currentCardPlayedIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerID, currentCardPlayedIndex)
    AddCardHealth(nextCreature, -1)
end
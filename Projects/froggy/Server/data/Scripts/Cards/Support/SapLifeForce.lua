function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerID, currentCardIndex)
    StealCardHealth(nextCreature, 2)

end
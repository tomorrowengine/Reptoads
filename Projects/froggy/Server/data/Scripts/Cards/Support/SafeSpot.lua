function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    RemoveRandomCardFromHand(playerID)

    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerID, currentCardIndex)
    AddCardHealth(nextCreature, -5)
end
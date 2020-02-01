function OnCardPlay(a_Card)
    playerID = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()

    nextCreature = GetNextCreature(playerID, currentCardIndex)
    AddCardHealth(nextCreature, -4)

end
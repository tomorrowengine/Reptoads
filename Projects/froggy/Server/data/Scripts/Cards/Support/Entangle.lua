function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerIndex, currentCardIndex)

    if (nextCreature ~= nil) then
        amountOfCards = GetHandSize(playerIndex)
        AddCardHealth(nextCreature, amountOfCards * -1)
    end
end
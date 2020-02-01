function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerIndex, currentCardIndex)

    if (nextCreature ~= nil) then
        AddCardHealth(nextCreature, -4)
    end
end
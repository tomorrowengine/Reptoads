function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreatureByFamily(playerIndex, currentCardIndex, "Kalphite")

    if (nextCreature ~= nil) then
        AddCardHealth(nextCreature, -5)
    end
end
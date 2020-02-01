function OnCardPlay(a_Card)
       
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreatureByFamily(playerIndex, currentCardIndex, "Undead")

    if (nextCreature ~= nil) then
        AddCardHealth(nextCreature, -8)
    end

end
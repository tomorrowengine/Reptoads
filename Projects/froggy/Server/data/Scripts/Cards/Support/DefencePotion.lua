function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreatureByFamily(playerIndex, currentCardIndex, "Undead")

    if (nextCreature ~= nil) then    
        AddCardAttack(nextCreature, -4)
    end
end
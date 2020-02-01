function OnCardPlay(a_Card)
       
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreatureByFamily(playerIndex, currentCardIndex, "Undead")

    if (nextCreature ~= nil) then
        nextCreatureAttack = GetCardAttack(nextCreature)
        if (nextCreatureAttack <= 4) then
            AddCardHealth(nextCreature, -4)
        end
    end
end
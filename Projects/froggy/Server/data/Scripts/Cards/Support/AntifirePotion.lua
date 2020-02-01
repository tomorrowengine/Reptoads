function OnCardPlay(a_Card)

    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreatureByFamily(playerIndex, currentCardIndex, "Dragon")

    if (nextCreature ~= nil) then
        AddCardAttack(nextCreature, -5)
    end
end
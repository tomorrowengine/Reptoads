function OnCardPlay(a_Card)

    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerIndex, currentCardIndex)
    
    RemoveRandomCardFromHand(playerIndex)

    if (nextCreature ~= nil) then
        AddCardAttack(nextCreature, -5)
    end
end
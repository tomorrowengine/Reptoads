function OnCardPlay(a_Card)
    
    rivalIndex = GetCurrentRivalID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(rivalIndex, currentCardIndex)

    if (nextCreature ~= nil) then
        AddCardAttack(nextCreature, 5)
        AddCardHealth(nextCreature, 3)
    end
end
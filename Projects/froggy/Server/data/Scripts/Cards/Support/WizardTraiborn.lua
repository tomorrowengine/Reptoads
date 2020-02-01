function OnCardPlay(a_Card)
    
    rivalID = GetCurrentRivalID()
    currentCardIndex = GetCardPlayedIndex()

    nextCreature = GetNextCreature(rivalID, currentCardIndex)
    AddCardHealth(nextCreature, 1)
    AddCardAttack(nextCreature, 3)
end
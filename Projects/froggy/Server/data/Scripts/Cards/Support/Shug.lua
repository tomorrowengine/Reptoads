function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerID, currentCardIndex)
    nextCreatureAttack = GetCardAttack(nextCreature)
    nextCreatureHealth = GetCardHealth(nextCreature)
    SetCardAttack(nextCreature, nextCreatureAttack)
    SetCardHealth(nextCreature, nextCreatureHealth)
end
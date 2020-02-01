function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID() 
    currentCardPlayedIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerID, currentCardPlayedIndex)
    nextCreatureAttack = GetCardAttack(nextCreature)
    playerHero = GetHero()
    AddHeroResource(playerHero, nextCreatureAttack / 2)

end
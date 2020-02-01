function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID() 
    currentCardPlayedIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerID, currentCardPlayedIndex)
    nextCreatureHealth = GetCardHealth(nextCreature)
    playerHero = GetHero()
    AddHeroHealth(playerHero, nextCreatureHealth / 2)

end
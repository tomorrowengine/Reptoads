function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    AddHeroArmor(playerHero, -3)

    playerID = GetCurrentPlayerID() 
    currentCardPlayedIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerID, currentCardPlayedIndex)
    AddCardHealth(nextCreature, -4)

end
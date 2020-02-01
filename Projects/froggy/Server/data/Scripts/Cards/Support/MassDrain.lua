function OnCardPlay(a_Card)

    playerID = GetCurrentPlayerID()  
    currentCardPlayedIndex = GetCardPlayedIndex()
    amountOfCreatures = AddHealthToCreaturesInPlay(playerID, currentCardPlayedIndex, -1)

    playerHero = GetHero()
    AddHeroHealth(playerHero, amountOfCreatures)
end
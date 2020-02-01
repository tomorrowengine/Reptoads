function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID
    rivalIndex = GetCurrentRivalID
    currentCardIndex = GetCardPlayedIndex()

    amountOfCreaturesPlayer = AddHealthToCreaturesInPlay(playerIndex, currentCardIndex, 1)
    amountOfCreaturesRival = AddHealthToCreaturesInPlay(rivalIndex, currentCardIndex, 1)

    playerHero = GetHero()
    AddHeroHealth(playerHero, (amountOfCreaturesPlayer + amountOfCreaturesRival) * -1)

end
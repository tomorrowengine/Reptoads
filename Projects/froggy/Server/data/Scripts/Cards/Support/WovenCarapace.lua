function OnCardPlay(a_Card)

    playerID = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    amountOfCreatureSlain = GetAmountOfCreatureSlainByFamily(playerID, currentCardIndex, "Kalphite")
    playerHero = GetHero()
    AddHeroArmor(playerHero, amountOfCreatureSlain * 5)
end
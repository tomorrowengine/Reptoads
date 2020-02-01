function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID
    currentCardIndex = GetCardPlayedIndex()
    amountOfCreaturesSlain = GetAmountOfCreatureSlainByFamily(playerIndex, currentCardIndex, "Beast")

        playerHero = GetHero()
    AddHeroHealth(playerHero, amountOfCreaturesSlain * 4)
end
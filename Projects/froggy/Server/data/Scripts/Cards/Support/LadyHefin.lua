function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    amountOfCards = GetAmountOfCardsInHandByType(playerIndex, "Creature")

    playerHero = GetHero()
    AddHeroHealth(playerHero, amountOfCards * 2)
end
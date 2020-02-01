function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    numberOfCards = GetAmountOfCardsInHandByFamily(playerIndex,"Kalphite")

    if (numberOfCards >= 2) then
        RemoveRandomCardFromHandByFamily(playerIndex, "Kalphite")
        playerHero = GetHero()
        AddHeroArmor(playerHero, 10)
    end
end
function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    amountOfCreatureSlain = GetAmountOfCreatureSlain(playerIndex, currentCardIndex)
    if (amountOfCreatureSlain == 3) then
        playerHero = GetHero()
        AddHeroHealth(playerHero, 5)
    end
end
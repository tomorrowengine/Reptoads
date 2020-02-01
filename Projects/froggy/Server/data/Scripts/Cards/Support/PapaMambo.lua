function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    amountOfCards = GetAmountOfCardsInHandByFamily(playerID, "Potion")

    if (amountOfCards >= 2) then
        RemoveCardFromHandByFamily(playerID, "Potion")
        rivalID = GetCurrentRivalID()
        AddHealthToCreaturesInPlay(rivalID, currentCardPlayedIndex, 4)
    end
end
function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    cardIndex = GetCardInHandByName(playerIndex, "Cannonball")

    if(cardIndex ~= -1) then
        RemoveCardFromHand(playerIndex, cardIndex)
        rivalIndex = GetCurrentRivalID()
        RemoveRandomCardsFromDeck(rivalIndex, 3)
    end
end
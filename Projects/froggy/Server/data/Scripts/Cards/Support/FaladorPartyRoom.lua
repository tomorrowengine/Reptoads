function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    DrawCardFromDeck(playerIndex, 4)
    RemoveRandomCardsFromHand(playerIndex, 2)
end
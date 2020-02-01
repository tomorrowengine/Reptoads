function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    rivalIndex = GetCurrentRivalID()

    DrawCardFromDeck(playerIndex, 1)
    DrawCardFromDeck(rivalIndex, 1)
end
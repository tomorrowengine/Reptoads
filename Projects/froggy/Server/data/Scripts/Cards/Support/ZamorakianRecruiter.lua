function OnCardPlay(a_Card)
    
    rivalID = GetCurrentRivalID()
    RemoveCardFromDeckByFamily(rivalID, "Ally")

    playerID = GetCurrentPlayerID()
    DrawCardFromDeck(playerID,1)
end
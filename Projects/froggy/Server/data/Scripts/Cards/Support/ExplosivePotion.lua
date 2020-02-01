function OnCardPlay(a_Card)
    
    rivalID = GetCurrentRivalID()

    RemoveRandomCardsFromDeckByFamily(rivalID, "Location", 1)
end
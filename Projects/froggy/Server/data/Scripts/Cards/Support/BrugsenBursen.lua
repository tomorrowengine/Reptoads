function OnCardPlay(a_Card)

    rivalID = GetCurrentRivalID()
    currentCardPlayedIndex = GetCardPlayedIndex()
    nextSupport = GetNextSupport(rivalID, currentCardPlayedIndex)
    AddCardCost(nextSupport, 4)
end
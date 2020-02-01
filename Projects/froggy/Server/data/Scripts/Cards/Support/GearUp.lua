function OnCardPlay(a_Card)
    
    rivalIndex = GetCurrentRivalID()
    RemoveAllCardsFromHand(rivalIndex)
    DrawCardFromDeck(rivalIndex, 5)

end
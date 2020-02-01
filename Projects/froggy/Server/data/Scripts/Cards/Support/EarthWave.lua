function OnCardPlay(a_Card)

    playerIndex = GetCurrentPlayerID()
    amountOfCards = GetHandSize(playerIndex)

    rivalHero = GetRival()
    AddHeroHealth(rivalHero, (amountOfCards + 4) * -1)
    RemoveAllCardsFromHand(playerIndex)
end
function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()

    rivalHero = GetRival()

    AddHeroResource(rivalHero, 4)
    DrawCardFromDeck(playerIndex, 3)

end
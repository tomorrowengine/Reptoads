function OnCardPlay(a_Card)

    rivalID = GetCurrentRivalID()

    playerHero = GetHero()
    handSize = GetHandSize(rivalID)
    AddHeroResource(playerHero, handSize)
end
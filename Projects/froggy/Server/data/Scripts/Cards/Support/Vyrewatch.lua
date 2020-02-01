function OnCardPlay(a_Card)

    playerHero = GetHero()
    rivalHero = GetRival()

    StealHeroHealth(rivalHero, playerHero, 5)

    rivalID = GetCurrentRivalID()
    RemoveRandomCardFromHand(rivalID)
end
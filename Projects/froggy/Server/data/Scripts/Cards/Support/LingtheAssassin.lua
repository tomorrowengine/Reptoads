function OnCardPlay(a_Card)

    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroResource = GetHeroResource(playerHero)

    DealDamageToHero(rivalHero, playerHeroResource / 2)
    AddHeroResource(playerHero, (playerHeroResource / 2) * -1)

end
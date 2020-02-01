function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroResource = GetHeroResource(playerHero)

    DealDamageToHero(rivalHero, playerHeroResource)

end
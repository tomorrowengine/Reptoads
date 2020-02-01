function OnCardPlay(a_Card)

    playerHero = GetHero()

    playerHeroArmor = GetHeroArmor(playerHero)
    AddHeroResource(playerHero, playerHeroArmor)
    AddHeroArmor(playerHero, playerHeroArmor * -1)

end
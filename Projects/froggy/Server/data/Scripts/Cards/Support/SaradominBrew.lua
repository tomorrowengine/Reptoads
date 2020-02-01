function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    playerHeroArmor = GetHeroArmor(playerHero)
    AddHeroHealth(playerHero,playerHeroArmor)

end
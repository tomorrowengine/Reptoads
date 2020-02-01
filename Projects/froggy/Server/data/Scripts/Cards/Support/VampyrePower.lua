function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    SetHeroArmor(playerHero, 0)
    SetHeroWeapon(playerHero, nil)
    AddHeroHealth(playerHero, 10)

end
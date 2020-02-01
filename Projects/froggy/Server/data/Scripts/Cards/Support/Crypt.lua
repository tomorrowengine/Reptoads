function OnCardPlay(a_Card)

    playerHero  = GetHero()

    playerHeroTotalAttack = GetHeroTotalAttack(playerHero)
    
    AddHeroHealth(playerHero,playerHeroTotalAttack)
    SetHeroWeapon(playerHero,nil)
end
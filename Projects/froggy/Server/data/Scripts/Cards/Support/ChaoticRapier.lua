function OnCardPlay(a_Card)
    
    playerHero  = GetHero()
    rivalHero   = GetRival()

    weapon = CreateHeroWeapon(1,3)
    SetHeroWeapon(playerHero, weapon)

    rivalHeroArmor = GetHeroArmor(rivalHero)

    AddWeaponAttack(playerHero, rivalHeroArmor/2)
end
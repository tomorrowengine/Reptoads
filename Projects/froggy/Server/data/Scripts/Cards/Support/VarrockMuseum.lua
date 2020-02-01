function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    playerHeroWeaponDurability = GetWeaponDurability(playerHero)
    AddHeroResource(playerHero, playerHeroWeaponDurability)
    SetHeroWeapon(playerHero, nil)
end
function OnCardPlay(a_Card)

    playerHero  = GetHero()

    playerHeroWeaponDurability = GetWeaponDurability(playerHero, -2)
    SetWeaponAttack(playerHero, playerHeroWeaponDurability)

end
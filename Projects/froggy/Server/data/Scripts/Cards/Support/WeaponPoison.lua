function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    playerHeroWeapon = GetHeroWeapon(playerHero)

    if (playerHeroWeapon ~= nil) then
        AddWeaponAttack(playerHero, 2)
        AddWeaponDurability(playerHero, -1)
    end
end
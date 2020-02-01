function OnCardPlay(a_Card)
    
    
    playerHero = GetHero()

    playerHeroWeapon = GetHeroWeapon(playerHero)

    if (playerHeroWeapon ~= nil) then
        
        playerHeroWeaponDurability = GetWeaponDurability(playerHero)
        playerHeroWeaponAttack = GetWeaponAttack(playerHero)
        
        if (playerHeroWeaponDurability >= playerHeroWeaponAttack) then

            AddHeroArmor(playerHero, 4)
            AddWeaponAttack(playerHero, 2)
        end
    end
end
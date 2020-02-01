function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    playerHeroWeapon = GetHeroWeapon(playerHero)
    if (playerHeroWeapon ~= nil) then

        playerHeroWeaponDurability = GetWeaponDurability(playerHero)
        playerHeroResource = GetHeroResource(playerHero)

        if (playerHeroResource >= playerHeroWeaponDurability) then

            AddHeroResource(playerHero, playerHeroWeaponDurability)
            AddWeaponDurability(playerHeroWeapon, playerHeroWeaponDurability)
        end
    end
end
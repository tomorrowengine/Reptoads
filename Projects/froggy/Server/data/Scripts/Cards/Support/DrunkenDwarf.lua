function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    playerHeroWeapon = GetHeroWeapon(playerHero)
    if (playerHeroWeapon ~= nil) then

        playerHeroWeaponDurability = GetWeaponDurability(playerHero)
        AddHeroHealth(playerHero, playerHeroWeaponDurability)
    end
end
function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    weapon = GetWeapon(playerHero)

    if (weapon ~= nil) then
        AddWeaponDurability(playerHero, 1)
        AddWeaponAttack(playerHero, 1)
    end
end
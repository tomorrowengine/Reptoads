function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    weapon = GetHeroWeapon(playerHero)

    if (weapon ~= nil) then
        AddWeaponDurability(playerHero, 3)
        print("Add 3 weapon durability")
    else
        print("No weapon to give durability")
    end
end
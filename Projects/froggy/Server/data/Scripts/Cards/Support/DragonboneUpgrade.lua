function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    weapon = GetWeapon(playerHero)

    if (weapon ~= nil) then
        AddWeaponDurability(playerHero, 1)
        AddWeaponAttack(playerHero, 1)
        print("Add 1 weapon durability")
        print("Add 1 weapon attack")
    end

    print("No weapon to give 1 durability and 1 attack ")
end
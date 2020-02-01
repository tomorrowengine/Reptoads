function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    weapon = GetHeroWeapon(playerHero)

    if (weapon ~= nil) then
        SetHeroWeapon(playerHero, nil)
        AddHeroResource(playerHero, 5)
    end
end
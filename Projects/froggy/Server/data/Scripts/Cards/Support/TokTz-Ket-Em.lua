function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    playerHeroArmor = GetHeroArmor(playerHero)

    if (playerHeroArmor >= 4) then
        AddHeroArmor(playerHero, -4)
        weapon = CreateHeroWeapon(3,6)
        SetHeroWeapon(playerHero, weapon)
    end
end
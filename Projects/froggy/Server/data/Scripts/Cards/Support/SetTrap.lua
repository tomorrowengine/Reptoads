function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroWeapon = GetHeroWeapon(playerHero)

    if (playerHeroWeapon ~= nil) then
        AddWeaponAttack(playerHero, -3)
    end

    AddHeroHealth(rivalHero, -3)
end
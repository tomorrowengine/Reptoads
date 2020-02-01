function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    rivalHeroWeapon = GetHeroWeapon(rivalHero)

    if (rivalHeroWeapon ~= nil) then
        AddHeroArmor(playerHero, 5)
    end

end
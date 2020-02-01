function OnCardPlay(a_Card)

    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroWeapon = GetHeroWeapon(playerHero)

    if (playerHeroWeapon ~= nil) then

        SetHeroWeapon(playerHero, nil)
        AddHeroArmor(playerHero, 8)
    end
end
function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroWeapon = GetHeroWeapon(playerHero)

    if (playerHeroWeapon ~= nil) then
        AddHeroAttack(rivalHero, -2)
        SetHeroWeapon(playerHero, nil)
    end
end
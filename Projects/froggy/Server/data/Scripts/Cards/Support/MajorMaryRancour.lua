function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    weapon = CreateHeroWeapon(1,1)
    SetHeroWeapon(rivalHero, weapon)

end
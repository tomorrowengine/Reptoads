function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    AddWeaponDurability(rivalHero, -5)
end
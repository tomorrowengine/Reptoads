function OnCardPlay(a_Card)
    
    rivalHero   = GetRival()

    DealDamageToHero(rivalHero,-3)
    AddWeaponDurability(rivalHero, -2)

end
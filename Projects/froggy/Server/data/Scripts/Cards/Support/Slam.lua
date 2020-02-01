function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()


    DealDamageToHero(rivalHero, 5)
    AddWeaponDuribility(rivalHero, -4)


end
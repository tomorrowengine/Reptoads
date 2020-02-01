function OnCardPlay(a_Card)
    
    rivalHero   = GetRival()
    
    rivalHeroHealth = GetHeroHealth(rivalHero)

    AddHeroArmor(rivalHero, -6)
    DealDamageToHero(rivalHero, 6)

end
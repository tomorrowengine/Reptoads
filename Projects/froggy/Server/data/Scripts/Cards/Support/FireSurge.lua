function OnCardPlay(a_Card)
    
    rivalHero = GetRival()
    rivalHeroArmor = GetHeroArmor(rivalHero)

    AddHeroHealth(rivalHero, (rivalHeroArmor / 2) * -1)

end
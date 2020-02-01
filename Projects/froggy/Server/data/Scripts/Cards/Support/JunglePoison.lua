function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    AddHeroHealth(rivalHero,GetHeroHealth(rivalHero) / 2)

end
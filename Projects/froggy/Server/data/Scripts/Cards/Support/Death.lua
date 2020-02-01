function OnCardPlay(a_Card)

    rivalHero   = GetRival()
    
    rivalHeroHealth = GetHeroHealth(rivalHero)

    AddHeroHealth(rivalHero,rivalHeroHealth / 2)
end
function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    StealHeroHealth(rivalHero, playerHero, 1)
end
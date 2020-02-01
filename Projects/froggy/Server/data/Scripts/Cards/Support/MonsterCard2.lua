function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    AddHeroHealth(playerHero, -2)
    AddHeroHealth(rivalHero, -2)
    
end
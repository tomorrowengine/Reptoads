function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    StealHeroArmor(rivalHero, playerHero, 5)
end
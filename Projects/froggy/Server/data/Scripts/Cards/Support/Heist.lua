function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    StealHeroArmor(rivalHero, playerHero, 5)

    print("Steal up to 5 armor from the rivalHero")
end
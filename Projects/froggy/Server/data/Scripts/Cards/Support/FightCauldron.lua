function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    AddHeroHealth(playerHero, -8)

    print("Added -8 health to the playerHero")
end
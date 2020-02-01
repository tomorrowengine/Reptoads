function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    AddHeroHealth(playerHero, -1)
    AddHeroHealth(rivalHero, -1)

    print("remove -1 health from your playerHero" )
    print("remove -1 health from your rivalHero" )
end
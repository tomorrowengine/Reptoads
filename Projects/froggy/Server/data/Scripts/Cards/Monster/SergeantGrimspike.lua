function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    AddHeroHealth(rivalHero, -6)

    print("Add -6 health to the rivalHero")
end
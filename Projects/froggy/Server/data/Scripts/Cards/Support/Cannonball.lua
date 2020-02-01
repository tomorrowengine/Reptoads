function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    DealDamageToHero(rivalHero, -1)

    print("Added -1 health to the rivalHero")
end
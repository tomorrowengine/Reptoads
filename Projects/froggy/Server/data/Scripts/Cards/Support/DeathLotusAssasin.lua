function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    DealDamageToHero(rivalHero, -8)

    print("Added -8 health to the rivalHero")
end
function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    DealDamageToHero(playerHero, -1)
    DealDamageToHero(rivalHero, -1)

    print("remove -1 health from your playerHero" )
    print("remove -1 health from your rivalHero" )
end
function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    AddHeroHealth(playerHero, GetHeroAttack(rivalHero))

    print("Add " .. GetHeroAttack(rivalHero) .. " Health to the hero" )
end
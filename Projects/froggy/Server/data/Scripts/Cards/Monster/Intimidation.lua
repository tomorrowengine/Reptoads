function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroArmor = GetHeroArmor(playerHero)
    rivalHeroArmor = GetHeroArmor(rivalHero)

    if (rivalHeroArmor > playerHeroArmor) then
        AddHeroAttack(rivalHero, 1)
        print("Add 1 attack to rivalHero")
    elseif (rivalHeroArmor < playerHeroArmor) then
        AddHeroAttack(playerHero, 1)
        print("Add 1 attack to playerHero")
    end

    print("Not one of the Heros has a heigher amount of armor then the other")
end
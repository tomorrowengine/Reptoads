function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    playerHeroArmor = GetHeroArmor()

    if (playerHeroArmor - 2 >= 0) then
        AddHeroArmor(playerHero,-2)
    else
        AddHeroArmor(playerHero,  playerHeroArmor)
    end
end
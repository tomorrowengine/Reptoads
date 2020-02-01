function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    playerHeroArmor = GetHeroArmor()

    if (playerHeroArmor - 2 >= 0) then
        AddHeroArmor(playerHero,-2)
        print("Added -2 armor to the playerHero")
    else
        AddHeroArmor(playerHero,  playerHeroArmor)
        print("Added " .. playerHeroArmor .. " armor to the playerHero")
    end
end
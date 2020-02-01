function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    playerHeroHealth = GetHeroHealth(playerHero)

    if (playerHeroHealth <= 15) then
        AddHeroArmor(playerHero, 12)
    end
end
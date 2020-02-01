function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    playerHeroHealth = GetHeroHealth(playerHero)

    if (playerHeroHealth <= 15) then
        AddHeroHealth(playerHero, playerHeroHealth)
    end
end
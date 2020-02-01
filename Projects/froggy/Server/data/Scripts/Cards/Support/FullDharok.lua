function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    playerHeroHealth = GetHeroHealth(playerHero)

    if (playerHeroHealth <= 15) then
        AddHeroAttack(playerHero, 2)
    end
end
function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroHealth = GetHeroHealth(playerHero)

    if (playerHeroHealth <= 15) then
        StealHeroHealth(rivalHero, playerHero, 6)
    else
        StealHeroHealth(rivalHero, playerHero, 3)
    end

end
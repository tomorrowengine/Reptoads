function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    playerHeroHealth = GetHeroHealth(playerHero)
    if (playerHeroHealth < 15) then
        SetHeroHealth(playerHero,15)
        print("PlayerHero health set to 15")
    else
        print("PlayerHero already has 15 or more health")
    end
end
function OnCardPlay(a_Card)
    
    playerHero  = GetHero()
    rivalHero   = GetRival()
    
    rivalHeroHealth = GetHeroHealth(rivalHero)

    if (rivalHeroHealth > 20) then
        DealDamageToHero(rivalHero,7)
    end
end
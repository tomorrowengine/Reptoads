function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroAttack = GetHeroAttack(playerHero)

    if (playerHeroAttack == 1) then
        AddHeroHealth(rivalHero, -6)
    end
end
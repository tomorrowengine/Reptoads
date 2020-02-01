function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    playerHeroAttack = GetHeroAttack(playerHero)

    if (playerHeroAttack == 1) then
        AddHeroAttack(playerHero, 2)
    end
end
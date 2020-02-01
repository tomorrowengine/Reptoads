function OnCardPlay(a_Card)

    playerHero  = GetHero()

    playerHeroArmor = GetHeroArmor(playerHero)

    if (playerHeroArmor >= 10) then
        AddHeroAttack(playerHero, 1)
    end

end
function OnCardPlay(a_Card)

    playerHero = GetHero()
    playerHeroArmor = GetHeroArmor(playerHero)

    if (playerHeroArmor >= 3) then
        AddHeroArmor(playerHero, -3)
        playerID = GetCurrentPlayerID()
        DrawCardFromDeck(playerID, 4)
    end
end
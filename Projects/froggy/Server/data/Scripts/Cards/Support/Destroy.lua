function OnCardPlay(a_Card)

    playerHero = GetHero()

    playerHeroArmor = GetHeroArmor(playerHero)

    if(playerHeroArmor >= 6) then

        AddHeroArmor(playerHero, -6)

        playerIndex = GetCurrentPlayerID()
        currentCardIndex = GetCardPlayedIndex()
        nextCreature = GetNextCreature(playerIndex, currentCardIndex)

        if (nextCreature ~= nil) then    
            SetCardHealth(nextCreature, 0)
        end
    end
end
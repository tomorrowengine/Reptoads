function OnCardPlay(a_Card)
    
    playerHero = GetHero()

    AddHeroHealth(playerHero, -2)

    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerIndex, currentCardIndex)

    if (nextCreature ~= nil) then
        playerHeroArmor = GetHeroArmor(playerHero)
        AddCardHealth(nextCreature,(playerHeroArmor / 2) * -1)
    end
end
function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerIndex, currentCardIndex)

    if (nextCreature ~= nil) then
        playerHero = GetHero()
        StealCardHealth(nextCreature, playerHero, 6)
    end
end
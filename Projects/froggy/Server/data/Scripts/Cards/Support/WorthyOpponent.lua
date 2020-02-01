function OnCardPlay(a_Card)
    playerID = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()

    nextCreature = GetNextCreature(rivalID, currentCardIndex)
    nextCreatureHealth = GetCardHealth(nextCreature)
    if (nextCreatureHealth >= 10) then
        playerHero = GetHero()
        AddHeroAttack(playerHero, 1)
    end
end
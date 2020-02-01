function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreaturePlayer = GetNextCreature(playerIndex, currentCardIndex)

    if (nextCreaturePlayer ~= nil) then
        nextCreaturePlayerHealth = GetCardHealth(nextCreaturePlayer)

        rivalIndex = GetCurrentRivalID()
        nextCreatureRival = GetNextCreature(rivalIndex, currentCardIndex)

        if (nextCreatureRival ~= nil) then
            AddCardHealth(nextCreatureRival,nextCreaturePlayerHealth)
        end
    end
end
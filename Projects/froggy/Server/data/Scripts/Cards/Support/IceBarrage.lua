require('CombatLibrary')

function OnCardPlay(a_Card)
    
    rivalHero = GetRival()
    DealDamageToHero(rivalHero, 5)

    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerIndex, currentCardIndex)

    if (nextCreature ~= nil) then
        AddCardHealth(nextCreature, -4)
    end

end
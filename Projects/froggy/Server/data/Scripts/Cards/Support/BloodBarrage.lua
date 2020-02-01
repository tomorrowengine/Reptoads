require('CombatLibrary')

function OnCardPlay(a_Card)

    playerIndex = GetCurrentRivalID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerIndex, currentCardIndex)

    rivalHero = GetRival()
    DealDamageToHero(rivalHero, 2)

    if (nextCreature ~= nil) then
        AddCardHealth(nextCreature, -2)
    end

end
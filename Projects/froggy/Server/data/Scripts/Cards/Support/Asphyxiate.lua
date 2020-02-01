require('CombatLibrary')

function OnCardPlay(a_Card)
    
    rivalIndex = GetCurrentRivalID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(rivalIndex, currentCardIndex)

    if (nextCreature ~= nil) then
        rivalHero = GetRival()
        nextCreatureAttack = GetCardAttack(nextCreature)
        DealDamageToHero(rivalHero, nextCreatureAttack * 2)
    end

end
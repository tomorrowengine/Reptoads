require('CombatLibrary')

function OnCardPlay(a_Card)
       
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreatureByFamily(playerIndex, currentCardIndex, "Undead")

    if (nextCreature ~= nil) then
        nextCreatureAttack = GetCardAttack(nextCreature)
        rivalHero = GetRival()
        DealDamageToHero(rivalHero, nextCreatureAttack)
    end
end
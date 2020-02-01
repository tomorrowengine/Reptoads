
require('CombatLibrary')
function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID() 
    currentCardPlayedIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerID, currentCardPlayedIndex)
    nextCreatureAttack = GetCardAttack(nextCreature)
    rivalHero = GetRival()
    DealDamageToHero(rivalHero, nextCreatureAttack)
end
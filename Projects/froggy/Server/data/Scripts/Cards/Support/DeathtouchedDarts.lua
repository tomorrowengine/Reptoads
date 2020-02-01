require('CombatLibrary')

function OnCardPlay(a_Card)
       
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreatureByFamily(playerIndex, currentCardIndex, "Undead")

    if (nextCreature ~= nil) then
        
        SetcardHealth(nextCreature, 0)
    end
end
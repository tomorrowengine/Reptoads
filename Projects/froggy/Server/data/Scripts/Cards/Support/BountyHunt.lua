require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerIndex, currentCardIndex)
    
    RemoveRandomCardFromHand(playerIndex)

    if (nextCreature ~= nil) then
        playerHero = GetHero()
        HeroStrikeCard(playerHero,nextCreature)
    end
end
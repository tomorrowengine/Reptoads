require('CombatLibrary')
function OnCardPlay(a_Card)
    playerID = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    amountOfCreatureSlain = GetAmountOfCreatureSlainByFamily(playerID, currentCardIndex, "Goblin")
    DrawFamilyCardFromDeck(playerID, amountOfCreatureSlain,"Goblin")
    rivalHero = GetRival()
    DealDamageToHero(rivalhero, amountOfCreatureSlain * 2)
end
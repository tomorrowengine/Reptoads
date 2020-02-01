function OnCardPlay(a_Card)
    playerID = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    amountOfCreatureSlain = GetAmountOfCreatureSlainByFamily(playerID, currentCardIndex, "Ogre")

    rivalID = GetCurrentRivalID()
    AddCardsToDeck(rivalID, "Bones", amountOfCreatureSlain * 2)
end
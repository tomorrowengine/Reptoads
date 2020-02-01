function OnCardPlay(a_Card)
    

    playerIndex = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()

    AddHealthToCreaturesInPlayByFamily(playerIndex, currentCardIndex, "Kalphites", -3)

end
function OnCardPlay(a_Card)

    playerIndex = GetCurrentPlayerID()
    RemoveRandomCardFromHand(playerIndex)
    AddRandomCardsToHandBasedOnFamily(playerIndex,"Potion", 2)
end
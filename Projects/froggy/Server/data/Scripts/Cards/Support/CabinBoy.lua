function OnCardPlay(a_Card)
    playerIndex = GetCurrentPlayerID()
    
    RemoveRandomCardFromHand(playerIndex)
    AddCardsToDeck(playerIndex,"Cannonball", 6)
end
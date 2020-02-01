function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    AddCardToHand(playerIndex, "BloodBarrage")
    AddCardToHand(playerIndex, "IceBarrage")
    AddCardToHand(playerIndex, "ShadowBarrage")
    AddCardToHand(playerIndex, "SmokeBarrage")

end
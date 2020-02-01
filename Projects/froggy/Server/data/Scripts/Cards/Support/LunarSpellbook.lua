function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    AddCardToHand(playerIndex, "Boost")
    --AddCardToHand(playerIndex, "BorrowedPower")
    AddCardToHand(playerIndex, "GroupHeal")
    AddCardToHand(playerIndex, "Vengeance")

end
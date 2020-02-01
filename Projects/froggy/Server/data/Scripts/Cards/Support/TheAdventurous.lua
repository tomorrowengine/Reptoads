function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    AddRandomCardsToHandBasedOnFamily(playerID, "Pirate",3)
end
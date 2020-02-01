function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    RemoveRandomCardsFromHand(playerID, 3)

    playerHero = GetHero()
    playerHeroResource = GetHeroResource(playerHero)
    AddHeroResource(playerHero, playerHeroResourcea)
end
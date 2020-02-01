function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    AddHeroHealth(playerHero, -2)
    playerID = GetCurrentPlayerID()
    DrawCardFromDeck(playerID, 3)
end
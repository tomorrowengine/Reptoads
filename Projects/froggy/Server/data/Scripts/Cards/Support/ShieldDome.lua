function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    handSize = GetHandSize(playerID)
    playerHero = GetHero()
    AddHeroArmor(playerHero, handSize / 2)

end
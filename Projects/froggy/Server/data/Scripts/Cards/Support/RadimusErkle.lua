function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    card = GetCardInDeckByFamily(playerID,"Location")

    if (card ~= nil) then
        playerHero = GetHero()
        AddHeroResource(playerHero, 4)
        DrawCardFromDeck(playerID, 2)
    end
end
function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    rivalHeroResource = GetHeroResource(rivalHero)
    if(rivalHeroResource > 0) then
        AddHeroResource(rivalHero, -3)
        playerID = GetCurrentPlayerID()
        rivalID = GetCurrentRivalID()

        AddCardToHand(playerID, "Rum")
        AddCardToHand(rivalID, "Rum")
    end
end
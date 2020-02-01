function OnCardPlay(a_Card)
       
    playerIndex = GetCurrentPlayerID()
    cardIndex = GetCardInHandByFamily(playerIndex, "Potion")

    if (cardIndex ~= -1) then
        RemoveCardFromHand(playerIndex,cardIndex)
        playerHero = GetHero()
        AddHeroHealth(playerHero, 7)
    end

end
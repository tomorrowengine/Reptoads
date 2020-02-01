require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID()
    cardIndex = GetAmountOfCardsInHandByName(playerIndex, "CannonBall")

    if (cardIndex ~= -1) then
        RemoveCardFromHand(playerIndex, cardIndex)
        rivalHero = GetRival()
        DealDamageToHero(rivalHero, 7)
    end
end
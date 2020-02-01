require('CombatLibrary')
function OnCardPlay(a_Card)
    playerID = GetCurrentPlayerID()

    cardID = GetCardInHandByFamily(playerID,"Potion")
    if (cardID ~= -1) then
        RemoveCardFromHand(playerID, cardID)
        rivalHero = GetRival()
        DealDamageToHero(rivalhero, 7)
    end
end

require('CombatLibrary')
function OnCardPlay(a_Card)
    
    playerID = GetCurrentPlayerID()
    cardID = GetCardInHandByName(playerID, "Cannonball")

    if (cardID ~= -1) then
        RemoveCardFromHand(playerID, cardID)
        rivalHero = GetRival()
        DealDamageToHero(rivalhero, 4)
        AddCardToHand(playerID, "StickySanders")
    end
end
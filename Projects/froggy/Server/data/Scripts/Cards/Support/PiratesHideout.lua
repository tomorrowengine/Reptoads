require('CombatLibrary')

function OnCardPlay(a_Card)
    playerIndex = GetCurrentPlayerID()
    AddCardsToHand(playerIndex, "CannonBall", 3)
    playerHero = GetHero()
    DealDamageToHero(playerHero, 3)
end
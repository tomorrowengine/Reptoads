require('CombatLibrary')

function OnCardPlay(a_Card)

    playerHero = GetHero()
    rivalHero = GetRival()

    HeroBattle(playerHero, rivalHero)

    playerIndex = GetCurrentPlayerID()
    DrawCardFromDeck(playerIndex)
end
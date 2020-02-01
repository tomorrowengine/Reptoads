require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID
    currentCardIndex = GetCardPlayedIndex()
    amountOfCreaturesSlain = GetAmountOfCreatureSlainByFamily(playerIndex, currentCardIndex, "Demon")

    if (amountOfCreaturesSlain >= 2) then
        playerHero = GetHero()
        rivalHero = GetRival()

        HeroStrikeHero(playerHero,rivalHero)
    end
end
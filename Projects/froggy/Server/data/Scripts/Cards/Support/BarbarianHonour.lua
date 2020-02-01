require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerIndex = GetCurrentPlayerID
    rivalIndex = GetCurrentRivalID

    currentCardIndex = GetCardPlayedIndex()
    amountOfCreaturesSlainPlayer = GetAmountOfCreatureSlain(playerIndex, currentCardIndex)
    amountOfCreaturesSlainRival = GetAmountOfCreatureSlain(rivalIndex, currentCardIndex)

    if (amountOfCreaturesSlainPlayer > amountOfCreaturesSlainRival) then
        rivalHero = GetRival()
        DealDamageToHero(rivalHero, 5)
    elseif (amountOfCreaturesSlainPlayer > amountOfCreaturesSlainRival)
        playerHero = GetHero()
        DealDamageToHero(playerHero, 5)
    end

end
function OnCardPlay(a_Card)

    playerID = GetCurrentPlayerID()
    currentCardIndex = GetCardPlayedIndex()
    nextCreature = GetNextCreature(playerID, currentCardIndex)
    AddCardAttack(nextCreature, -2)
    rivalHero = GetRival()
    AddHeroAttack(rivalHero, -1)
end
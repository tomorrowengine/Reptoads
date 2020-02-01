require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        playerID = GetCurrentPlayerID()
        cardPlayedIndex = GetCardPlayedIndex()
        AddHealthToCreaturesInPlay(playerID, cardPlayedIndex, -1)
        AddAttackToCreaturesInPlay(playerID, cardPlayedIndex, -1)
    end
end
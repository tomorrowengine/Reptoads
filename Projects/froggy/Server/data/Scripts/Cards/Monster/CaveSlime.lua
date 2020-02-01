require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        playerID = GetCurrentPlayerID()
        currentplayedCard = GetCardPlayedIndex()
        nextCreature = GetNextCreature(playerID,currentplayedCard)
        AddCardHealth(nextCreature, -3)
    end
end
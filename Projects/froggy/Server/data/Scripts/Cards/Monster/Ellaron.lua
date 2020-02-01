require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        AddHealthToCreaturesInPlay(GetCurrentPlayerID(), GetCardPlayedIndex(), 1)
        AddHealthToCreaturesInPlay(GetCurrentRivalID(), GetCardPlayedIndex(), 1)
    end
end
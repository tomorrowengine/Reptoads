require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        local nextCard = GetNextCreatureByFamily(GetCurrentPlayerID(), GetCurrentCardIndex(), "Vampyre")
        AddCardAttack(nextCard, -4)
    end
end
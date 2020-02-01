require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        player = GetCurrentPlayerID()
        card = GetNextCreature(player,GetCardPlayedIndex())
        AddCardBaseAttack(card,3)
        rival = GetCurrentRivalID()
        card = GetNextCreature(rival,GetCardPlayedIndex())
        AddCardBaseAttack(card,3)
    end
end
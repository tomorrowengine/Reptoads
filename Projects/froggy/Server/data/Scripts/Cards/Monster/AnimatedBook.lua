require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        AddHeroAttack(hero,-1)
        DrawFamilyCardFromDeck(GetCurrentPlayerID(),2,"Spell")
    end
end
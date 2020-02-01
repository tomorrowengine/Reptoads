require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    CardStrikeHero(hero,a_Card)
    Combat(hero,a_Card)
end
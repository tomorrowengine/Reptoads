require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rival = GetRival()
        rivalResources = GetHeroResource(rival)
        AddHeroResource(hero, rivalResources / 2)
    end
end
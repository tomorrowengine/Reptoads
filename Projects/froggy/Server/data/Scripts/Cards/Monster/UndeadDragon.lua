require('CombatLibrary')
function OnCardPlay(a_Card)
    
    Hero = GetHero()
    Combat(Hero,a_Card)
    
    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rivalHero = GetRival()
        AddHeroHealth(rivalHero, -10)
    end
end
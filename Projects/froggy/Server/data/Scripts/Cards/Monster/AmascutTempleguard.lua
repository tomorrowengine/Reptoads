require('CombatLibrary')
function OnCardPlay(a_Card)
    
    Hero = GetHero()
    Combat(Hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        AddHeroHealth(Hero,-4)
        AddHeroResource(Hero,GetHeroResource(Hero))
    end
end
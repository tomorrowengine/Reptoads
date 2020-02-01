require('CombatLibrary')
function OnCardPlay(a_Card)
    
    Hero = GetHero()
    Combat(Hero, a_Card)

    StealHeroResource(GetRival(),Hero,2)
end
require('CombatLibrary')
function OnCardPlay(a_Card)
    
    Hero = GetHero()
    Combat(Hero,a_Card)

    Resource = GetHeroResource(Hero)
    if(Resource <= 4) then
        AddHeroHealth(Hero,4)
    else
        AddHeroHealth(Hero,Resource)
    end
end
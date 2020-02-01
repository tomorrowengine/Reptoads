require('CombatLibrary')
function OnCardPlay(a_Card)
    
    Hero = GetHero()
    Combat(Hero,a_Card)

    health = GetHeroHealth(Hero)
    if(health <= 15) then
        DealDamageToHero(GetRival(),8)
    else
        DealDamageToHero(GetRival(),4)
    end
end
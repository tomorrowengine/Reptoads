require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rival = GetRival()
        HeroStrikeHero(hero,rival)
        HeroStrikeHero(rival,hero)
        StealHeroHealth(rival,hero,2)
    end
end
require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rival = GetRival()
        rivalAttack = GetHeroTotalAttack(rival)
        heroAttack = GetHeroTotalAttack(hero)
        if(rivalAttack > heroAttack) then
            AddHeroAttack(hero,1)
        end
    end
end
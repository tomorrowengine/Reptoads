require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        attack = GetHeroAttack(hero)
        if(attack == 1) then
            rival = GetRival()
            AddHeroHealth(rival,-6)
        end
    end
end
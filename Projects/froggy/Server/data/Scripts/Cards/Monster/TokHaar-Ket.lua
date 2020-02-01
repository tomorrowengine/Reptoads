require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        armor = GetHeroArmor(hero)
        if(armor >= 4) then
            AddHeroArmor(hero,-4)
            AddHeroAttack(hero,2)
        end
    end
end
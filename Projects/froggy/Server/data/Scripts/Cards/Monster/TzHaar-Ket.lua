require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rival = GetRival()
        rivalArmor = GetHeroArmor(rival)
        armor = GetHeroArmor(hero)
        if(armor > rivalArmor) then
            SetHeroArmor(rival,0)
            AddHeroHealth(rival,-6)
        end
    end
end
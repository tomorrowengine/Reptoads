require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rival = GetRival()
        weapon = GetHeroWeapin(rival)
        if(weapon ~= nil) then
            AddWeaponAttack(weapon,-1)
        end
    end
end
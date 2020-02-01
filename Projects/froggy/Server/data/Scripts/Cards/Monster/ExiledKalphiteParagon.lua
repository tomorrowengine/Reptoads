require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rival = GetRival()
        weapon = GetHeroWeapon(rival)
        if(weapon ~= nil) then
            AddWeaponAttack(weapon,-2)
            AddWeaponDurability(weapon,-2)
        end
    end
end
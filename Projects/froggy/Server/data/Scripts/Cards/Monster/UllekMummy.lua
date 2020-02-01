require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rival = GetRival()
        weapon = GetHeroWeapon(her)
        if(weapon ~= nil) then
            weaponAttack = GetWeaponAttack(weapon)
            AddHeroResource(hero,weaponAttack)
        end
    end
end
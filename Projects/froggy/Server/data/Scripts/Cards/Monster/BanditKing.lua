require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rival = GetRival()
        weaponrival = GetHeroWeapon(rival)
        weapon = GetHeroWeapon(hero)
        StealHeroResources(rival,hero,1)
        if(weapon ~= nil) then
            if(weaponrival ~= nil) then
                AddWeaponAttack(weaponrival,-1)
                AddWeaponDurability(weaponrival,-1)
                AddWeaponAttack(weapon,1)
                AddWeaponDurability(weapon,1)
            end
        end
    end
end
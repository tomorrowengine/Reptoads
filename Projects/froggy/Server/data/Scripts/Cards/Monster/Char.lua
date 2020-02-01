require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        weapon = GetHeroWeapon(hero)
        if(weapon ~= nil) then
            weaponAttack = GetWeaponAttack(hero)
            AddHeroAttack(hero, weaponAttack / 2)
            SetHeroWeapon(hero,nil)
        end
    end
end
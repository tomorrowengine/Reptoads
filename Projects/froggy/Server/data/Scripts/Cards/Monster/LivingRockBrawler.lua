require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        weapon = GetHeroWeapon(hero)
        if(weapon ~= nil) then
            AddWeaponAttack(weapon,1)
            AddWeaponDurability(weapon,1)
        end
    end
end
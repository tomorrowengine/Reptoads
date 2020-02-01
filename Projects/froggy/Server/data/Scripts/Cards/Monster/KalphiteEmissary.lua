require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rival = GetRival()
        AddHeroArmor(rival,-5)
        weapon = GetHeroWeapon(rival)
        if(weapon ~= nil) then 
            AddWeaponDurability(weapon,-3)
        end
    end
end
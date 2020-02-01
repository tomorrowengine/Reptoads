require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        rival = GetRival()
        rivalArmor = GetHeroArmor(rival)
        weapon = GetHeroWeapon(hero)
        if(weapon ~= nil) then
            AddWeaponDurability(weapon,rivalArmor)
        end
    end
end
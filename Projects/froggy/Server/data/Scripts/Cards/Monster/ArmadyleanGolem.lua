require('CombatLibrary')
function OnCardPlay(a_Card)
    
    Hero = GetHero()
    Combat(Hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        weapon = GetHeroWeapon(Hero)
        if(weapon ~= nil) then
            attack = GetWeaponAttack(weapon)
            AddHeroArmor(Hero,attack)
        end
        AddHeroArmor(Hero,3)
    end
end
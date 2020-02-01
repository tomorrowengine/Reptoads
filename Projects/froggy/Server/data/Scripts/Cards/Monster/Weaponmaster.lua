require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        weapon = CreateHeroWeapon(3,3)
        rival = GetRival()
        rivalAttack = GetHeroAttack(rival)
        AddWeaponAttack(weapon,rivalAttack)
        SetHeroWeapon(hero,weapon)
    end
end
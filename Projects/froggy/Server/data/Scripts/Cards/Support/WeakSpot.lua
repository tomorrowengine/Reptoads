require('CombatLibrary')

function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    rivalHeroWeapon = GetHeroWeapon(rivalHero)

    if (rivalHeroWeapon == nil) then
        DealDamageToHero(rivalHero, 4)
    end
end
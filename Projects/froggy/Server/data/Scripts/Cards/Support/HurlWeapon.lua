require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroWeapon = GetHeroWeapon(playerHero)

    if (playerHeroWeapon ~= nil) then
        playerHeroWeaponDurability = GetWeaponDurability(playerHeroWeapon)
        DealDamageToHero(rivalHero, playerHeroWeaponDurability * 2)
        SetHeroWeapon(playerHero, nil)
    end
end
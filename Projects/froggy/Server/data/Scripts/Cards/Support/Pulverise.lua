require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroWeapon = GetHeroWeapon(playerHero)

    if (playerHeroWeapon ~= nil) then

        playerHeroWeaponAttack = GetWeaponAttack(playerHero)
        DealDamageToHero(rivalHero, playerHeroWeaponAttack * 2)
        AddWeaponAttack(playerHero, 3)
    end
end
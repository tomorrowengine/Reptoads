require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero  = GetHero()
    rivalHero   = GetRival()

    AddWeaponDurability(playerHero, -2)
    AddWeaponDurability(rivalHero, -2)

    HeroStrikeHero(playerHero, rivalHero)
    HeroStrikeHero(rivalHero, playerHero)

end
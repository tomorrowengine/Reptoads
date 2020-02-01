require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero  = GetHero()
    rivalHero   = GetRival()

    HeroStrikeHero(playerHero, rivalHero)
    StealHeroWeapon(rivalHero, playerHero)

end
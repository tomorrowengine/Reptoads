require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    HeroStrikeHero(playerHero, rivalHero)
    StealHeroArmor(rivalHero, playerHero, 5)

end
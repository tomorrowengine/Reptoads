require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    HeroStrikeHero(playerHero,rivalHero)
    StealHeroResource(rivalHero,playerHero, 3)

end
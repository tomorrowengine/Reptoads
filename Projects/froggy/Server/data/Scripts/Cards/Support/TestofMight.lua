require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    HeroStrikeHero(playerHero, rivalHero)
    HeroStrikeHero(rivalHero, playerHero)
end
require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    DealDamageToHero(rivalHero, 2)
    HeroStrikeHero(playerHero, rivalHero)
    HeroStrikeHero(rivalHero, playerHero)

end
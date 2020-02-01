require('CombatLibrary')

function OnCardPlay(a_Card)

    playerHero  = GetHero()
    rivalHero   = GetRival()

    DealDamageToHero(rivalHero, -1)
    HeroStrikeHero(playerHero, rivalHero)
end
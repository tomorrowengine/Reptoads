require('CombatLibrary')
function OnCardPlay(a_Card)
    
    playerHero  = GetHero()
    rivalHero   = GetRival()

    DealDamageToHero(playerHero, GetHeroTotalAttack(rivalHero))
    StealHeroResource(rivalHero, playerHero, 3)
end
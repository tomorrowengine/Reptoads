require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    rivalHeroAttack = GetHeroAttack(rivalHero)
    
    DealDamageToHero(rivalHero, 4)
    DealDamageToHero(playerHero, rivalHeroAttack)

end
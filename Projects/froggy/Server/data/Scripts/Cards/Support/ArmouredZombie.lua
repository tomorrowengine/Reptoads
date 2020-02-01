require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    monsterAttack = GetCardAttack(a_Card)
    DealDamageToHero(playerHero, monsterAttack)
    DealDamageToHero(rivalHero, monsterAttack)

end
require('CombatLibrary')

function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    DealDamageToHero(rivalHero, 4)

end
require('CombatLibrary')

function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    DealDamageToHero(rivalHero, 5)
end
require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    DealDamageToHero(playerHero, 2)
    DealDamageToHero(rivalHero, 2)
    
end
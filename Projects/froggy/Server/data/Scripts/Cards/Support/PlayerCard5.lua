require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()
    currentMonster = GetMonster()

    DealDamageToCard(currentMonster, 4)   
    DealDamageToHero(rivalHero, 2)
end
require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroArmor = GetHeroArmor(playerHero)
    DealDamageToHero(rivalHero, playerHeroArmor/2)

end
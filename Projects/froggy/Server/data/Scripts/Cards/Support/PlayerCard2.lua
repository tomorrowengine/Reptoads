require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    playerHeroHealth = GetHeroHealth(playerHero)
    print(playerHeroHealth)
    currentMonster = GetMonster()

    DealDamageToCard(currentMonster, 4)   
end
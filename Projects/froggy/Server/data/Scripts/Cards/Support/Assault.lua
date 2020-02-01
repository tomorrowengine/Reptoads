require('CombatLibrary')

function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    armor = GetHeroArmor(playerHero)

    if (armor >= 2) then
        AddHeroArmor(playerHero, -2)
        DealDamageToHero(rivalHero, -4)
        print("remove -2 armor from your playerHero" )
        print("remove -4 health from your rivalHero" )
    else
        print("Not enough armor to do effect")
    end
end
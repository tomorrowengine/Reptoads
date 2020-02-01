function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    rivalHeroTotalAttack = GetHeroTotalAttack(rivalHero)

    AddHeroArmor(playerHero, rivalHeroTotalAttack * 2)

end
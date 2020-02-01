function OnCardPlay(a_Card)
    
    playerHero  = GetHero()
    rivalHero   = GetRival()

    rivalHeroAttack = GetHeroAttack(rivalHero)
    AddHeroResource(playerHero, rivalHeroAttack)

end
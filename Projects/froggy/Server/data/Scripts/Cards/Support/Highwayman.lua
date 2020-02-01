function OnCardPlay(a_Card)
    
    playerHero = GetHero()
    rivalHero = GetRival()

    StealHeroWeapon(rivalHero, playerHero)
end
function OnCardPlay(a_Card)

    rivalHero   = GetRival()
    rivalHeroResource = GetHeroResource(rivalHero)

    AddHeroHealth(rivalHero, rivalHeroResource/2)
end
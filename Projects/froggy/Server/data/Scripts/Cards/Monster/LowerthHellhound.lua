function OnCardPlay(a_Card)
    hero = GetHero()
    Combat(hero,a_Card)
    heroHealth = GetHeroHealth(hero)
end
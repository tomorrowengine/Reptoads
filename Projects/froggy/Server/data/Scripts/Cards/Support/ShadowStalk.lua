require('CombatLibrary')

function OnCardPlay(a_Card)

    playerHero = GetHero()
    rivalHero = GetRival()

    playerHeroHealth = GetHeroHealth(playerHero)

    if (playerHeroHealth <= 15) then
        HeroStrikeHero(playerHero, rivalHero)
    else
        HeroStrikeHero(playerHero, rivalHero)
        HeroStrikeHero(rivalHero, playerHero)
    end
end
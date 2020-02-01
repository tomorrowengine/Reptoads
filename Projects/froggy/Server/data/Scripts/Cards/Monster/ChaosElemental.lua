require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        playerHero = GetHero()
        rivalHero = GetRival()
        SetHeroWeapon(playerHero, nil)
        SetHeroArmor(rivalHero, 0)
    end
end
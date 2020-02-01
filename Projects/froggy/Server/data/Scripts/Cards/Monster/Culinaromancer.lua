require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        playerHealth = GetHeroHealth(hero)
        if(playerHealth <= 15) then
            AddHeroHealth(hero,6)
        end
    end
end
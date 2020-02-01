require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        weapon = GetHeroWeapon(hero)
        if(weapon ~= nil) then
            SetHeroWeapon(nil)
            player = GetCurrentPlayerID()
            DrawCardFromDeck(player,4)
        end
    end
end
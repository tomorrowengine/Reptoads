require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        player = GetCurrentPlayerID()
        card = GetNextCreature(player,GetCardPlayedIndex)
        AddCardBaseHealth(card,-4)
        rival = GetRival()
        AddHeroHealth(rival,-4)
    end
end
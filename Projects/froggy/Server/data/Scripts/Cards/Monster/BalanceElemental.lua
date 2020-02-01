require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        player = GetCurrentPlayerID()
        RemoveCardsFromHand(player)
        DrawCardFromDeck(player,5)
        rival = GetCurrentRivalID()
        RemoveCardsFromHand(rival)
        DrawCardFromDeck(rival,5)
    end
end
require('CombatLibrary')
function OnCardPlay(a_Card)
    
    hero = GetHero()
    Combat(hero,a_Card)

    health = GetCardHealth(a_Card)
    if (health <= 0) then
        player = GetCurrentPlayerID()
        allies = GetAmountOfCardsInHandByFamily(player,"Ally")
        if(allies >= 4) then
            rival = GetRival()
            AddHeroHealth(rival,-15)
            RemoveRandomCardFromHandByFamily(rival,"Ally")
        end
    end
end
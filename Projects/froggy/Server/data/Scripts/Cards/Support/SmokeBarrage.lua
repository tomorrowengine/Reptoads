require('CombatLibrary')
function OnCardPlay(a_Card)
    
    rivalHero = GetRival()

    DealDamageToHero(rivalHero, 3)
    rivalID = GetCurrentRivalID()
    RemoveRandomCardFromHand(rivalID)
end
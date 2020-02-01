function OnCardPlay(a_Card)
    

    playerHero = GetHero()
    playerHeroWeapon = GetHeroWeapon(playerHero)

    if(playerHeroWeapon ~= nil) then
        playerHeroWeaponAttack = GetWeaponAttack(playerHero)
        rivalID = GetCurrentRivalID()
        currentCardIndex = GetCardPlayedIndex()

        nextCreature = GetNextCreature(rivalID, currentCardIndex)
        AddCardHealth(nextCreature, 2)
        AddCardAttack(nextCreature, playerHeroWeaponAttack)
        SetHeroWeapon(playerHero, nil)
    end
end
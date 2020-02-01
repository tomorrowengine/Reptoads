require('CombatLibrary')
function OnCardPlay(a_Card)
    
    Hero = GetHero()
    Combat(Hero,a_Card)

    if(GetHeroWeapon(Hero) ~= nil) then
        Rival = GetRival()
        AddWeaponDurability(Hero,GetHeroResource(Rival))
    end
    
end
function OnCardPlay(a_Card)
    
    rivalHero = GetRival()
	
    AddHeroHealth(rivalHero, -1)
end
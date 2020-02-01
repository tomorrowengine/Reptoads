function UseWeapon(a_Hero)
    weapon = GetHeroWeapon(a_Hero)
    if(weapon ~= nil) then
        attack = GetWeaponAttack(a_Hero)
        durability = GetWeaponDurability(a_Hero)
        if(durability > 0) then
            SetWeaponDurability(a_Hero, durability - 1)
            return attack
        else
            return 0
        end
    else
        return 0
    end
end

function ArmorHit(a_Attack,a_Hero)
    armor = GetHeroArmor(a_Hero)
    if(armor > 0) then
        if(armor >= a_Attack) then
            SetHeroArmor(a_Hero,armor - a_Attack)
            return 0
        elseif(armor < a_Attack) then
            SetHeroArmor(a_Hero,0)
            return a_Attack - armor
        end
    else
        return a_Attack
    end
end

function DealDamageToHero(a_Hero,a_Attack)
    health = GetHeroHealth(a_Hero)
    remainingAttack = ArmorHit(a_Attack,a_Hero)
    remainingHealth = health - remainingAttack

    if(remainingHealth <= 0) then
        SetHeroHealth(a_Hero,0)
    else
        SetHeroHealth(a_Hero,remainingHealth)
    end
end

function DealDamageToCard(damage, receiver)
    
    receiverArmor = GetCardArmor(receiver)

    if (receiverArmor > 0) then
        newArmor = receiverArmor - damage;
        if (newArmor < 0) then
            damage = receiverArmor;
            SetCardArmor(receiver,0) 
        else    
            AddCardArmor(receiver,-damage)
        end
    end

    receiverHealth = GetCardHealth(receiver)

    if (receiverHealth > 0 and damage > 0) then
        newHealth = receiverHealth - damage;
        if (newHealth < 0) then
            SetCardHealth(receiver,0)
        else
            AddCardHealth(receiver, -damage)
        end
    end
end

function HeroStrikeHero(a_Hero,a_HeroTarget)
    attack = GetHeroAttack(a_Hero)
    weaponAttack = UseWeapon(a_Hero)
    health = GetHeroHealth(a_HeroTarget)
    remainingAttack = ArmorHit(attack + weaponAttack,a_HeroTarget)
    SetHeroHealth(a_HeroTarget,health - remainingAttack)
    if(health - remainingAttack <= 0) then
        SetHeroHealth(a_HeroTarget,0)
    end
end

function HeroStrikeCard(a_Hero,a_Card)
    attack = GetHeroAttack(a_Hero)
    weaponAttack = UseWeapon(a_Hero)
    health = GetCardHealth(a_Card)
    remainingAttack = attack + weaponAttack
    SetCardHealth(a_Card,health - remainingAttack)
    if(health - remainingAttack <= 0) then
        SetCardHealth(a_Card,0)
    end
end

function CardStrikeHero(a_Hero,a_Card)
    attack = GetCardAttack(a_Card)
    health = GetHeroHealth(a_Hero)
    remainingAttack = ArmorHit(attack + weaponAttack,a_HeroTarget)
    SetHeroHealth(a_Hero,health - remainingAttack)
    if(health - remainingAttack <= 0) then
        SetHeroHealth(a_Hero,0)
    end
end

function HeroBattle(a_Hero,a_Rival)
    HeroStrikeHero(a_Hero,a_Rival)
    health = GetHeroHealth(a_Rival)
    if(health > 0) then
        HeroStrikeHero(a_Rival,a_Hero)
    end
end

function Combat(a_Hero,a_Card)
    while(true) do
        attack = GetHeroAttack(a_Hero)
        weaponAttack = UseWeapon(a_Hero)
        health = GetCardHealth(a_Card)
        remainingAttack = attack + weaponAttack
        if(remainingAttack > 0) then
            SetCardHealth(a_Card,health - remainingAttack)
            if(health - remainingAttack <= 0) then
                SetCardHealth(a_Card,0)
                break
            end
        end
        attack = GetCardAttack(a_Card)
        health = GetHeroHealth(a_Hero)
        remainingAttack = ArmorHit(attack,a_Hero)
        if(remainingAttack > 0) then
            SetHeroHealth(a_Hero,health - remainingAttack)
            if(health - attack <= 0) then
                SetHeroHealth(a_Hero,0)
                break
            end
        end
    end
end

function HeroFight(a_Hero1,a_Hero2)
    while(true) do
        attack = GetHeroAttack(a_Hero1)
        weaponAttack = UseWeapon(a_Hero1)
        health = GetHeroHealth(a_Hero2)
        remainingAttack = ArmorHit(attack + weaponAttack,a_Hero2)
        if(remainingAttack > 0) then
            SetHeroHealth(a_Hero2,health - remainingAttack)
            if(health - remainingAttack <= 0) then
                SetHeroHealth(a_Hero2,0)
                break
            end
        end
        attack = GetHeroAttack(a_Hero2)
        weaponAttack = UseWeapon(a_Hero2)
        health = GetHeroHealth(a_Hero1)
        remainingAttack = ArmorHit(attack + weaponAttack,a_Hero1)
        if(remainingAttack > 0) then
            SetHeroHealth(a_Hero1,health - remainingAttack)
            if(health - remainingAttack <= 0) then
                SetHeroHealth(a_Hero1,0)
                break
            end
        end
    end  
end
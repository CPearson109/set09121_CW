// MageStats.h
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>


class MageStats {
public:
    MageStats(float health, float mana, float attackSpeed, float attackDamage)
        : health(health), mana(mana), attackSpeed(attackSpeed), attackDamage(attackDamage) {}

    // Getter methods for accessing the attributes
    float getHealth() const { return health; }
    float getMana() const { return mana; }
    float getAttackSpeed() const { return attackSpeed; }
    float getAttackDamage() const { return attackDamage; }

private:
    float health;
    float mana;
    float attackSpeed;
    float attackDamage;
};

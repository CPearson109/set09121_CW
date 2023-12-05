// EntityStats.h
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>


class EntityStats {
public:
    EntityStats() : health(0), movementSpeed(0), attackSpeed(0), attackDamage(0) {}
    EntityStats(float health, float movementSpeed, float attackSpeed, float attackDamage)
        : health(health), movementSpeed(movementSpeed), attackSpeed(attackSpeed), attackDamage(attackDamage) {}

    // Getter methods for accessing the attributes
    float getHealth() const { return health; }
    float getMovementSpeed() const { return movementSpeed; }
    float getAttackSpeed() const { return attackSpeed; }
    float getAttackDamage() const { return attackDamage; }

    void increaseHealth(float amount) {
        health += amount;
        // Add any necessary checks for maximum health if required
    }

    void increaseMovementSpeed(float amount) {
        movementSpeed += amount;
        // Add any necessary checks for maximum speed if required
    }

    // Add methods for attack speed and damage as needed
    void increaseAttackSpeed(float amount) {
        attackSpeed += amount;
        // Add any necessary checks for maximum attack speed if required
    }

    void increaseAttackDamage(float amount) {
        attackDamage += amount;
        // Add any necessary checks for maximum attack damage if required
    }

private:
    float health;
    float movementSpeed;
    float attackSpeed;
    float attackDamage;
};

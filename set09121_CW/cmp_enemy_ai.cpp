#include "cmp_enemy_ai.h"
#include "LevelSystem.h"
#include <cmath>
#include <iostream>

using namespace sf;

EnemyAIComponent::EnemyAIComponent(Entity* p, std::function<sf::Vector2f()> getPlayerPosition)
    : ActorMovementComponent(p), _getPlayerPosition(getPlayerPosition) {}

void EnemyAIComponent::update(double dt) {
    const auto mva = (float)(dt * _speed);
    const sf::Vector2f pos = _parent->getPosition();

    // Get the player's current position
    sf::Vector2f playerPos = _getPlayerPosition();

    // Calculate direction towards the player
    Vector2f direction = playerPos - pos;
    if (length(direction) > 0) {
        direction = normalize(direction);
    }

    // Move towards the player
    const Vector2f newpos = pos + direction * mva;
    if (LevelSystem::getTileAt(newpos) != LevelSystem::WALL) {
        move(direction * mva);
    }
    else {
        // Handle collision with wall
    }

    ActorMovementComponent::update(dt);
}

// Utility function to normalize a vector
Vector2f normalize(const Vector2f& v) {
    float len = length(v);
    if (len != 0) {
        return v / len;
    }
    return v;
}

// Utility function to calculate the length of a vector
float length(const Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

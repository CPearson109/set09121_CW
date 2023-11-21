#include "cmp_enemy_ai.h"
#include "LevelSystem.h"
#include "pacman.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>

using namespace sf;

// Constructor
EnemyAIComponent::EnemyAIComponent(Entity* p, Entity& player)
    : ActorMovementComponent(p), _player(player) {}

// Update method
void EnemyAIComponent::update(double dt) {
    const auto mva = (float)(dt * _speed);
    const Vector2f pos = _parent->getPosition();

    // Get the player's position from the game scene
    auto playerPos = _player.getPosition();

    // Print the player's position to the console
    std::cout << "Player Position: X=" << playerPos.x << " Y=" << playerPos.y << std::endl;

    // Calculate direction towards the player
    Vector2f direction = playerPos - pos;
    if (length(direction) > 0) {
        direction = normalize(direction);
    }



    const Vector2f newpos = pos + direction * mva;

    // Check if next position is a wall
    if (LevelSystem::getTileAt(newpos) != LevelSystem::WALL) {
        move(direction * mva);
    }
    else {
        // Handle case when ghost hits a wall
        // For example, stop or choose a new random direction
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

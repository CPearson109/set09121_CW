#pragma once
#include "SFML/Graphics.hpp"
#include "ecm.h"
#include "cmp_sprite.h"
#include "cmp_actor_movement.h" // Include the actor movement component header

class Player : public Entity {
private:
    float _speed;
    std::shared_ptr<ActorMovementComponent> _movement; // Keep a shared pointer to the ActorMovementComponent

public:
    void Update(double dt);
    Player();
    void Render(sf::RenderWindow& window) const;
};

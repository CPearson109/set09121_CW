#pragma once
#include "SFML/Graphics.hpp"
#include "ecm.h"
#include "cmp_sprite.h"
#include "cmp_actor_movement.h"
#include "player_projectile.h"

class Player : public Entity {
private:
    float _speed;
    sf::RenderWindow& _window; // Reference to the game window
    sf::Texture& _projectileTexture; // Reference to the projectile texture 
    std::shared_ptr<ActorMovementComponent> _movement;

public:
    Player(sf::RenderWindow& window, sf::Texture& projectileTexture);
    void Update(double dt);
    void Render(sf::RenderWindow& window) const;
    void FireProjectile();
};

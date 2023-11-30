#pragma once
#include "SFML/Graphics.hpp"
#include "ecm.h"
#include "cmp_sprite.h"
#include "cmp_player_movement.h"

class Player : public Entity {
private:
    float _speed;
    sf::RenderWindow& _window;
    sf::Texture& _projectileTexture;
    std::shared_ptr<ActorMovementComponent> _movement;
    float _projectileCooldown;
    float _timeSinceLastShot;

public:
    Player(Scene* scene, sf::RenderWindow& window, sf::Texture& projectileTexture);
    void update(double dt) override;
    void FireProjectile();
};

class Enemy : public Entity {
private:
    float _speed;

public:
    void Update(double dt);
    Enemy();
    void Render(sf::RenderWindow& window) const;
};
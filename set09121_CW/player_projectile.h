//player_projectile.h
#pragma once
#include <SFML/Graphics.hpp>
#include "ecm.h"

class Projectile : public Entity {
private:
    sf::Vector2f _direction;
    float _speed;
    float _lifetime; // How long the projectile lives before disappearing

public:
    Projectile(const sf::Vector2f& pos, const sf::Vector2f& direction, float speed);
    void update(double dt) override;
    void setLifetime(float lifetime);
};

// player_projectile.h
#pragma once
#include <SFML/Graphics.hpp>
#include "ecm.h"

class Projectile : public Entity {
private:
    sf::Vector2f _direction;
    float _speed;
    float _lifetime;
    sf::Texture& _projectileTexture; // Add texture reference

public:
    Projectile(Scene* scene, const sf::Vector2f& pos, const sf::Vector2f& direction, float speed, sf::Texture& texture);
    void update(double dt) override;
    void setLifetime(float lifetime);
};


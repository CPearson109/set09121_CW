#pragma once
#include "SFML/Graphics.hpp"
#include "ecm.h"
#include "cmp_sprite.h"
#include "cmp_player_movement.h"

class Player : public Entity {
private:
    float _speed;
    sf::RenderWindow& _window;
    std::shared_ptr<ActorMovementComponent> _movement;

public:
    Player(Scene* scene, sf::RenderWindow& window);
    void update(double dt) override;
};

class Enemy : public Entity {
private:
    float _speed;

public:
    void Update(double dt);
    Enemy();
    void Render(sf::RenderWindow& window) const;
};
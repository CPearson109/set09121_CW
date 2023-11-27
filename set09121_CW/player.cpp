// player.cpp
#include "player.h"
#include "system_renderer.h"
#include "player_movement_component.h"
#include "player_projectile.h"
#include "game.h"

using namespace sf;

Player::Player(Scene* scene, sf::RenderWindow& window, sf::Texture& projectileTexture)
    : Entity(scene), _window(window), _projectileTexture(projectileTexture), _speed(10.f), _projectileCooldown(0.5f), _timeSinceLastShot(0.f) {}

void Player::update(double dt) {
    _timeSinceLastShot += dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && _timeSinceLastShot >= _projectileCooldown) {
        FireProjectile();
        _timeSinceLastShot = 0.f;
    }

    Entity::update(dt);
}

void Player::FireProjectile() {

    // Fixed starting position (0,0)
    sf::Vector2f startPosition(100.f, 100.f);

    // Fixed direction (example: moving upwards)
    sf::Vector2f direction(0.f, -1.f);

    auto projectile = std::make_shared<Projectile>(scene, startPosition, direction, 300.f, _projectileTexture);
    scene->addEntity(projectile);
}
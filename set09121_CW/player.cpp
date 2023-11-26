// player.cpp
#include "player.h"
#include "system_renderer.h"
#include "player_movement_component.h"
#include "player_projectile.h"
#include "game.h"

using namespace sf;

Player::Player(sf::RenderWindow& window, sf::Texture& projectileTexture)
    : _window(window), _projectileTexture(projectileTexture), _speed(100.f) {
}

void Player::Update(double dt) {
    auto movement = get_components<PlayerMovementComponent>()[0];

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        fireProjectile();
    }

    Entity::update(dt);
}

void Player::fireProjectile() {
    auto window = /* get your SFML window reference */;
    sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    sf::Vector2f direction = mousePos - getPosition();
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= magnitude; // Normalize the direction vector

    auto projectile = std::make_shared<Projectile>(getPosition(), direction, 300.f); // Speed of projectile
    // Add the projectile to the scene or entity manager
}
#include "actors.h"
#include "system_renderer.h"
#include "cmp_player_movement.h"
#include "game.h"

using namespace sf;

// Player methods
Player::Player(Scene* scene, sf::RenderWindow& window)
    : Entity(scene), _window(window), _speed(10.f) {}

void Player::update(double dt) {
    Entity::update(dt);
    // Additional player update code
}

// Enemy methods
void Enemy::Update(double dt) {
    auto movement = get_components<ActorMovementComponent>()[0];
    Entity::update(dt);
    // Additional enemy update code
}

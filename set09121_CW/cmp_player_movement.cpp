//"cmp_player_movement.cpp"
#include "cmp_player_movement.h"
#include "cmp_enemy_ai.h"
#include <LevelSystem.h>

using namespace sf;

ActorMovementComponent::ActorMovementComponent(Entity* p)
    : _speed(100.f), Component(p) {}

bool ActorMovementComponent::validMove(const sf::Vector2f& pos) {
    return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
}

void ActorMovementComponent::move(const sf::Vector2f& p) {
    auto pp = _parent->getPosition() + p;
    if (validMove(pp)) {
        _parent->setPosition(pp);
    }
}

void ActorMovementComponent::move(float x, float y) {
    move(Vector2f(x, y));
}
float ActorMovementComponent::getSpeed() const { 
    return _speed; 
}

void ActorMovementComponent::setSpeed(float speed) { 
    _speed = speed; 
}

void ActorMovementComponent::update(double dt) {
    float speed = getSpeed() * dt;

    if (Keyboard::isKeyPressed(Keyboard::W)) {
        move(Vector2f(0.f, -speed));
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        move(Vector2f(-speed, 0.f));
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        move(Vector2f(0.f, speed));
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        move(Vector2f(speed, 0.f));
    }
}
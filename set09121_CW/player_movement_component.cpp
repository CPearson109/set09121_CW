#include "player_movement_component.h"
#include <SFML/Window/Keyboard.hpp>

using namespace sf;

PlayerMovementComponent::PlayerMovementComponent(Entity* p)
    : ActorMovementComponent(p) {}

void PlayerMovementComponent::update(double dt) {
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
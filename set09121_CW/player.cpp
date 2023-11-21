// player.cpp
#include "player.h"
#include "system_renderer.h"
#include "PlayerMovementComponent.h" // Update this include to use the PlayerMovementComponent

using namespace sf;

void Player::Update(double dt) {
    auto movement = get_components<PlayerMovementComponent>()[0]; // Use PlayerMovementComponent
    // Rest of the method stays the same
    Entity::update(dt); // Call the base update
}

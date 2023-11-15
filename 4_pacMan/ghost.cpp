// ghost.cpp (incorrectly labeled as player.cpp in the comment)
#include "ghost.h"
#include "system_renderer.h"
#include "cmp_actor_movement.h"

using namespace sf;

void Ghost::Update(double dt) {

    auto movement = get_components<ActorMovementComponent>()[0]; // Assuming Ghost has an ActorMovementComponent
    float speed = movement->getSpeed() * dt;


    Entity::update(dt); // Call the base update
}
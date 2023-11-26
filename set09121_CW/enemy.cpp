// Enemy.cpp
#include "enemy.h"
#include "system_renderer.h"
#include "cmp_actor_movement.h"

using namespace sf;

void Enemy::Update(double dt) {

    auto movement = get_components<ActorMovementComponent>()[0];

    Entity::update(dt);
}
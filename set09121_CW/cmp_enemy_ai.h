#pragma once
#include "cmp_actor_movement.h"
#include "pacman.h"
#include <SFML/Graphics.hpp>

class EnemyAIComponent : public ActorMovementComponent {
protected:
    Entity& _player;

public:
    EnemyAIComponent(Entity* p, Entity& player);
    void update(double dt) override;
};
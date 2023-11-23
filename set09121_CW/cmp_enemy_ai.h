#pragma once
#include <functional>
#include "cmp_actor_movement.h"
#include "ecm.h"

class EnemyAIComponent : public ActorMovementComponent {
protected:
    std::function<sf::Vector2f()> _getPlayerPosition;

public:
    EnemyAIComponent(Entity* p, std::function<sf::Vector2f()> getPlayerPosition);
    void update(double dt) override;
};
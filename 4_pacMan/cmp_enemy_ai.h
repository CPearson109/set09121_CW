#pragma once
#include "cmp_actor_movement.h"
#include <SFML/Graphics.hpp>

class EnemyAIComponent : public ActorMovementComponent {
protected:
	sf::Vector2f _direction;
	enum state { ROAMING, ROTATING, ROTATED };
	state _state;

public:
	explicit EnemyAIComponent(Entity* p, const sf::Vector2f& direction);

	void update(double dt) override;
};

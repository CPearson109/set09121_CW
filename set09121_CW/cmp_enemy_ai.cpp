#include "cmp_enemy_ai.h"
#include "LevelSystem.h"
#include <SFML/System/Vector2.hpp>
#include <cstdlib>

using namespace sf;

static const Vector2i directions[] = { {1, 0}, {0, 1}, {0, -1}, {-1, 0} };

EnemyAIComponent::EnemyAIComponent(Entity* p, const sf::Vector2f& direction)
    : ActorMovementComponent(p), _direction(direction), _state(ROAMING) {}

void EnemyAIComponent::update(double dt) {
    const auto mva = (float)(dt * _speed);
    const Vector2f pos = _parent->getPosition();
    const Vector2f newpos = pos + _direction * mva;
    const Vector2i baddir = -1 * Vector2i(_direction);
    Vector2i newdir = directions[(rand() % 4)];

    switch (_state) {
    case ROAMING:
        if (LevelSystem::getTileAt(newpos) == LevelSystem::WALL ||
            LevelSystem::getTileAt(newpos) == LevelSystem::WAYPOINT) {
            _state = ROTATING;
        }
        else {
            move(_direction * mva);
        }
        break;

    case ROTATING: {
        bool foundNewDirection = false;
        int attempts = 0;
        while (!foundNewDirection && attempts < 10) {
            newdir = directions[(rand() % 4)];
            if (newdir != baddir &&
                LevelSystem::getTileAt(pos + Vector2f(newdir)) != LevelSystem::WALL) {
                foundNewDirection = true;
            }
            attempts++;
        }
        if (!foundNewDirection) {
            newdir = baddir; // Reverse direction if no other option
        }
        _direction = Vector2f(newdir);
        _state = ROTATED;
        break;
    }

    case ROTATED:
        if (LevelSystem::getTileAt(pos) != LevelSystem::WAYPOINT) {
            _state = ROAMING;
        }
        move(_direction * mva);
        break;
    }

    ActorMovementComponent::update(dt);
}

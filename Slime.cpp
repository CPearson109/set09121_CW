//Slime.cpp
#include "Slime.h" 
#include "Game.h"
#include "Bullet.h"
#include "LevelSystem.h"
#include "EntityStats.h"
#include "Entity.h"
#include "Mage.h"

sf::Vector2ul safeConvertToVector2ul2(const sf::Vector2f& vec) {
    sf::Vector2ul result;
    result.x = (vec.x >= 0) ? static_cast<unsigned long>(vec.x) : 0;
    result.y = (vec.y >= 0) ? static_cast<unsigned long>(vec.y) : 0;
    return result;
}

Slime::Slime(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture, Entity& player, float speed, const EntityStats& slimeStats)
    : Entity(ir, pos, texture), _entityStats(slimeStats),  // Initialize _entityStats

    _player(player),
    idleAnimation(0, 0, 49, 62, 3, 0.2f),
    attackAnimation(0, 62, 49, 62, 3, 0.2f),
    movingAnimation(0, 2 * 62, 49, 62, 3, 0.2f),
    _speed(speed),
    _shootTimer(0.0f),
    _health(100)

{
    currentAnimation = &idleAnimation;
    setOrigin(ir.width / 2.0f, ir.height / 2.0f);
}


void Slime::onBulletHit(bool isPlayerBullet) {
    if (isPlayerBullet == true) {
        _health -= 50;
        std::cerr << _health << std::endl;
        if (_health <= 0) {
            _isActive = false;
            setPosition(sf::Vector2f(-100.f, -100.f));
        }
    }
}

void Slime::FireBullet(bool mode, const sf::Vector2f& direction, const sf::Vector2f& pos, bool isPlayerBullet) {


    // Calculate the position below the Mage's head
    sf::Vector2f bulletPosition = pos;

    bulletPosition.x += getLocalBounds().width / 2.0f; // Center horizontally
    bulletPosition.y += getLocalBounds().height / 2.0f; // Center vertically

    // Fire the bullet from the calculated position
    Bullet::Fire(bulletPosition, mode, direction, isPlayerBullet);
}

void Slime::Update(const float& dt, sf::RenderWindow& window) {
    // Update the current animation
    //currentAnimation->Update(dt);
    //currentAnimation->ApplyToSprite(*this);


    sf::Vector2f previousPosition = getPosition();

    // Calculate the direction vector towards the player for movement
    sf::Vector2f playerPos = _player.getPosition();
    sf::Vector2f direction = playerPos - getPosition();
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize the direction vector if the magnitude is non-zero
    if (magnitude > 0) {
        direction /= magnitude;
    }

    bool pathClear = false;
    float angleAdjustment = 0.1f; // Start with a small angle adjustment
    sf::Vector2f adjustedDirection = direction;

    // Try adjusting the direction until a clear path is found or a full circle is made
    for (float angle = 0; angle < 4 * 3.14159f; angle += angleAdjustment) {
        // Rotate the direction
        adjustedDirection = sf::Vector2f(std::cos(angle) * direction.x - std::sin(angle) * direction.y,
            std::sin(angle) * direction.x + std::cos(angle) * direction.y);

        // Check for walls in the adjusted direction
        bool wallInPath = false;
        sf::Vector2f toPlayer = playerPos - getPosition();
        float steps = std::max(std::abs(toPlayer.x), std::abs(toPlayer.y)) / LevelSystem::getTileSize();
        sf::Vector2f step = adjustedDirection * LevelSystem::getTileSize();

        for (int i = 0; i < steps; i++) {
            sf::Vector2f checkPos = getPosition() + step * static_cast<float>(i);
            sf::Vector2ul tilePos = safeConvertToVector2ul2(checkPos / LevelSystem::getTileSize());
            if (LevelSystem::getTile(tilePos) == LevelSystem::WALL) {
                wallInPath = true;
                break;
            }
        }

        if (!wallInPath) {
            pathClear = true;
            break; // Path is clear in this direction
        }
    }

    // If a clear path is found, move in that direction
    if (pathClear) {
        move(adjustedDirection * _speed * dt);
    }

    _shootTimer += dt;

    // Check if cooldown is over
    if (_shootTimer >= 3.0f) { // 1 second cooldown
        sf::Vector2f bulletDirection = _player.getPosition() - getPosition();
        float bulletMagnitude = std::sqrt(bulletDirection.x * bulletDirection.x + bulletDirection.y * bulletDirection.y);
        if (bulletMagnitude > 0) {
            bulletDirection /= bulletMagnitude;
        }

        bool isPlayerBullet = false;

        FireBullet(false, bulletDirection, getPosition(), isPlayerBullet);

        // Reset the shoot timer
        _shootTimer = 0.0f;
    }

    // New position after moving
    setPosition(sf::Sprite::getPosition());
    sf::Vector2f newPosition = getPosition();

    // Handle horizontal collisions
    sf::Vector2ul tilePosH(static_cast<unsigned>(newPosition.x / LevelSystem::getTileSize()),
        static_cast<unsigned>(previousPosition.y / LevelSystem::getTileSize()));
    if (LevelSystem::getTile(tilePosH) == LevelSystem::WALL) {
        newPosition.x = previousPosition.x;
    }

    // Handle vertical collisions
    sf::Vector2ul tilePosV(static_cast<unsigned>(previousPosition.x / LevelSystem::getTileSize()),
        static_cast<unsigned>(newPosition.y / LevelSystem::getTileSize()));
    if (LevelSystem::getTile(tilePosV) == LevelSystem::WALL) {
        newPosition.y = previousPosition.y;
    }

    setPosition(newPosition);
}
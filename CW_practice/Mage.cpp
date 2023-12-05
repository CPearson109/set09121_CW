//Mage.cpp
#include "Game.h"
#include "Bullet.h"
#include "LevelSystem.h"
#include "EntityStats.h"
#include "Mage.h"
#include "Entity.h"
#include "Slime.h"

Mage::Mage(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture, const EntityStats& mageStats)
    : Entity(ir, pos, texture), _stats(mageStats), // Initialize _entityStats

    walkDownAnimation(0, 0, 35, 37, 4, 0.2f), // Set the sprite size to 35x37 pixels
    walkUpAnimation(0, 38, 35, 37, 4, 0.2f), // Assuming each row is 37 pixels apart
    walkRightAnimation(0, 75, 35, 37, 4, 0.2f), // 2 rows down
    walkLeftAnimation(0, 112, 35, 37, 4, 0.2f), // Set the sprite size to 35x37 pixels
    speed(100.f), // Set this to your desired speed
    _shootTimer(1.0f)
{
    setPosition(pos);

    // Set the origin of the sprite to its center
    sf::FloatRect bounds = getLocalBounds();
    setOrigin(ir.width / 2.0f, ir.height / 2.0f);

    // Set the spritesheet texture to the Mage sprite
    setTexture(spritesheet);
    setTextureRect(ir);
}

void Mage::onBulletHit(bool isPlayerBullet) {
    if (!isPlayerBullet) {

        _stats.increaseHealth(-10);
        std::cerr << _stats.getHealth() << std::endl;
        if (_stats.getHealth() <= 0) {
            _isPlayerDead = true;
        }
    }
}

void Mage::resetPlayer() {
    _isPlayerDead = false;
    _stats = EntityStats(100.f, 100.f, 1.0f, 10.f);
    setPosition(LevelSystem::getStartTilePosition());
}

bool Mage::isPlayerDead() const {
    return _isPlayerDead;
}

void Mage::FireBullet(bool mode, const sf::Vector2f& direction, const sf::Vector2f& pos) {

    bool isPlayerBullet = true;
    // Calculate the position below the Mage's head
    sf::Vector2f bulletPosition = pos;

    bulletPosition.x += getLocalBounds().width / 2.0f; // Center horizontally
    bulletPosition.y += getLocalBounds().height / 2.0f; // Center vertically

    // Fire the bullet from the calculated position
    Bullet::Fire(bulletPosition, mode, direction, isPlayerBullet);
}

void Mage::Update(const float& dt, sf::RenderWindow& window) {
    // Store the current position before moving
    sf::Vector2f previousPosition = getPosition();

    // Update shoot timer
    _shootTimer += dt;

    // Handle bullet firing
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && _shootTimer >= 1.0f) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
        sf::Vector2f direction = worldMousePos - getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length != 0) { // Normalize the direction vector
            direction /= length;
        }

        FireBullet(false, direction, getPosition());
        _shootTimer = 0.0f;
    }

    // Handle keyboard input for movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        currentDirection = Direction::Up;
        move(0, -speed * dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        currentDirection = Direction::Down;
        move(0, speed * dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        currentDirection = Direction::Left;
        move(-speed * dt, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        currentDirection = Direction::Right;
        move(speed * dt, 0);
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

    // Update animation based on the direction
    switch (currentDirection) {
    case Direction::Down:
        walkDownAnimation.Update(dt);
        walkDownAnimation.ApplyToSprite(*this);
        break;
    case Direction::Up:
        walkUpAnimation.Update(dt);
        walkUpAnimation.ApplyToSprite(*this);
        break;
    case Direction::Left:
        walkRightAnimation.Update(dt);
        walkRightAnimation.ApplyToSprite(*this);
        setScale(-1.f, 1.f); // Flip the sprite horizontally 
        break;
    case Direction::Right:
        walkRightAnimation.Update(dt);
        walkRightAnimation.ApplyToSprite(*this);
        setScale(1.f, 1.f); // Reset the sprite scale 
        break;
    }
}
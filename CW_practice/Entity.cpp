// Entity.cpp
#include "Entity.h" 
#include "Game.h"
#include "Bullet.h"
#include "LevelSystem.h"

using namespace sf;
using namespace std;


Entity::Entity() {};

//constructor with an initialization list
Entity::Entity(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture) : _sprite(), _texture(texture) {
    _sprite = ir;
    setTexture(spritesheet);
    setTextureRect(_sprite);
    setPosition(pos);
    // Initialize other member variables if needed
}
const sf::Vector2f& Entity::getPosition() const {
    return _position;
}

void Entity::setPosition(const sf::Vector2f& position) {
    _position = position;
    sf::Sprite::setPosition(position);
}

void Entity::Update(const float& dt, sf::RenderWindow& window) {}

Entity::~Entity() {}



Slime::Slime(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture, Entity& player, float speed)
    : Entity(ir, pos, texture),
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

void Slime::onBulletHit() {
    _health -= 50; // Reduce health by 50

    if (_health <= 0) {
        // Destroy the Slime
        // For example, you might mark the Slime as inactive, remove it from the game world, 
        // or add it to a list of entities to be deleted
        //this->markForDeletion(); // This is a hypothetical function
        // OR
        // Game::removeEntity(this); // If you have a global game manager
        // OR
        // setActive(false); // If your entities can be deactivated
    }
}

void Slime::FireBullet(bool mode, const sf::Vector2f& direction, const sf::Vector2f& pos) {


    // Calculate the position below the Mage's head
    sf::Vector2f bulletPosition = pos;

    bulletPosition.x += getLocalBounds().width / 2.0f; // Center horizontally
    bulletPosition.y += getLocalBounds().height / 2.0f; // Center vertically

    // Fire the bullet from the calculated position
    Bullet::Fire(bulletPosition, mode, direction);
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
    for (float angle = 0; angle < 4  * 3.14159f; angle += angleAdjustment) {
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
            sf::Vector2ul tilePos(static_cast<unsigned>(checkPos.x / LevelSystem::getTileSize()),
                static_cast<unsigned>(checkPos.y / LevelSystem::getTileSize()));
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
    if (_shootTimer >= 1.0f) { // 1 second cooldown
        sf::Vector2f bulletDirection = _player.getPosition() - getPosition();
        float bulletMagnitude = std::sqrt(bulletDirection.x * bulletDirection.x + bulletDirection.y * bulletDirection.y);
        if (bulletMagnitude > 0) {
            bulletDirection /= bulletMagnitude;
        }

        FireBullet(false, bulletDirection, getPosition());

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




Mage::Mage(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture)
    : Entity(ir, pos, texture),
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

void Mage::FireBullet(bool mode, const sf::Vector2f& direction, const sf::Vector2f& pos) {


    // Calculate the position below the Mage's head
    sf::Vector2f bulletPosition = pos;

    bulletPosition.x += getLocalBounds().width / 2.0f; // Center horizontally
    bulletPosition.y += getLocalBounds().height / 2.0f; // Center vertically

    // Fire the bullet from the calculated position
    Bullet::Fire(bulletPosition, mode, direction);
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

// Entity.cpp
#include "Entity.h" 
#include "Game.h"
#include "Bullet.h"
#include "LevelSystem.h" // Include any additional necessary headers

using namespace sf;
using namespace std;


Entity::Entity() {};

//constructor with an initialization list
Entity::Entity(sf::IntRect ir, sf::Vector2f pos) : _sprite() {
    _sprite = ir;
    setTexture(spritesheet);
    setTextureRect(_sprite);
    setPosition(pos);
    // Initialize other member variables if needed
}


void Entity::Update(const float& dt) {}

Entity::~Entity() {
    // Implementation of pure virtual destructor, even if it's empty
}

bool Entity::is_exploded() const {
    return _exploded;
}

void Entity::Explode() {
    setTextureRect(IntRect(Vector2i(128, 32), Vector2i(32, 32)));
    _exploded = true;
}

Mage::Mage(sf::IntRect ir, sf::Vector2f pos)
    : Entity(),
    walkDownAnimation(0, 0, 35, 37, 4, 0.2f), // Set the sprite size to 35x37 pixels
    walkUpAnimation(0, 38, 35, 37, 4, 0.2f), // Assuming each row is 37 pixels apart
    walkRightAnimation(0, 75, 35, 37, 4, 0.2f), // 2 rows down
    walkLeftAnimation(0, 112, 35, 37, 4, 0.2f), // Set the sprite size to 35x37 pixels
    speed(100.f) // Set this to your desired speed
{
    setPosition(pos);

    // Set the origin of the sprite to its center
    sf::FloatRect bounds = getLocalBounds();
    setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Set the spritesheet texture to the Mage sprite
    setTexture(spritesheet);
    setTextureRect(ir);
}

void Mage::FireBullet(bool mode) {
    // Debug statement to indicate this method is called
    std::cout << "Mage::FireBullet called" << std::endl;


    // Calculate the position below the Mage's head
    sf::Vector2f bulletPosition = getPosition();
    bulletPosition.x += getLocalBounds().width / 2.0f; // Center horizontally
    bulletPosition.y += getLocalBounds().height / 2.0f; // Center vertically

    // Fire the bullet from the calculated position
    Bullet::Fire(bulletPosition, mode);
}

void Mage::Update(const float& dt) {
    // Store the current position before moving
    sf::Vector2f currentPosition = getPosition();

    // Store the current direction before handling keyboard input
    Direction previousDirection = currentDirection;


    // Handle keyboard input for movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        currentDirection = Direction::Up;

        // Calculate the next position if moving up
        sf::Vector2f nextPosition = currentPosition;
        nextPosition.y -= speed * dt;

        // Check for collisions with walls at the next position
        sf::Vector2ul tilePosition(static_cast<unsigned>(nextPosition.x / 35), static_cast<unsigned>(nextPosition.y / 35));
        if (LevelSystem::getTile(tilePosition) != LevelSystem::WALL) {
            // Move up only if there's no wall at the next position
            move(0, -speed * dt);
            setScale(1.f, 1.f); // Reset the sprite's scale
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        currentDirection = Direction::Down;

        // Calculate the next position if moving down
        sf::Vector2f nextPosition = currentPosition;
        nextPosition.y += speed * dt;

        // Check for collisions with walls at the next position
        sf::Vector2ul tilePosition(static_cast<unsigned>(nextPosition.x / 35), static_cast<unsigned>(nextPosition.y / 35));
        if (LevelSystem::getTile(tilePosition) != LevelSystem::WALL) {
            // Move down only if there's no wall at the next position
            move(0, speed * dt);
            setScale(1.f, 1.f); // Reset the sprite's scale
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        currentDirection = Direction::Left;

        // Calculate the next position if moving left
        sf::Vector2f nextPosition = currentPosition;
        nextPosition.x -= speed * dt;

        // Check for collisions with walls at the next position
        sf::Vector2ul tilePosition(static_cast<unsigned>(nextPosition.x / 35), static_cast<unsigned>(nextPosition.y / 35));
        if (LevelSystem::getTile(tilePosition) != LevelSystem::WALL) {
            // Move left only if there's no wall at the next position
            move(-speed * dt, 0);
            // Flip the sprite horizontally when moving left
            setScale(-1.f, 1.f);
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        currentDirection = Direction::Right;

        // Calculate the next position if moving right
        sf::Vector2f nextPosition = currentPosition;
        nextPosition.x += speed * dt;

        // Check for collisions with walls at the next position
        sf::Vector2ul tilePosition(static_cast<unsigned>(nextPosition.x / 35), static_cast<unsigned>(nextPosition.y / 35));
        if (LevelSystem::getTile(tilePosition) != LevelSystem::WALL) {
            // Move right only if there's no wall at the next position
            move(speed * dt, 0);
            // Set the sprite scale back to normal when moving right
            setScale(1.f, 1.f);
        }
    }

    // Check if the direction has changed and reset the animation frame if it has
    if (currentDirection != previousDirection) {
        switch (currentDirection) {
        case Direction::Down:
            walkDownAnimation.Reset(); // Reset the animation frame for down
            break;
        case Direction::Up:
            walkUpAnimation.Reset(); // Reset the animation frame for up
            break;
        case Direction::Left:
            walkLeftAnimation.Reset(); // Reset the animation frame for left
            break;
        case Direction::Right:
            walkRightAnimation.Reset(); // Reset the animation frame for right
            break;
        }
    }


    // Check for collisions with walls
    sf::Vector2ul tilePosition(static_cast<unsigned>(currentPosition.x / 35), static_cast<unsigned>(currentPosition.y / 35));

    if (LevelSystem::getTile(tilePosition) == LevelSystem::WALL) {
        // Reset the position to the previous position to prevent passing through walls
        setPosition(currentPosition);
    }

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
        walkRightAnimation.Update(dt); // Use walkRightAnimation when moving left
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

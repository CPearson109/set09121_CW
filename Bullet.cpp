// Include the necessary headers from SFML, and standard input-output library
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Bullet.h"
#include "Game.h"
#include "Entity.h"
#include "LevelSystem.h"
#include "Slime.h"
#include "Mage.h"

// Use the namespaces from SFML and standard library for convenience
using namespace sf;
using namespace std;

// Static variables for the Bullet class
unsigned char Bullet::bulletPointer = 0; // A static pointer to keep track of bullets
Bullet Bullet::bullets[64];             // Static array to store bullets
std::vector<Entity*> entities;            // Global vector to store entities
const float Bullet::bulletSpeed = 200.0f;  // Assign a value (e.g., 200.0f)

std::vector<Slime>* Bullet::_slimes = nullptr;
Mage* Bullet::_mage = nullptr;



// Default constructor for Bullet class, initializes a bullet as inactive and off-screen
Bullet::Bullet() : _active(false), _mode(false), _isPlayerBullet(false) {
    setTexture(bulletTexture); // Ensure texture is set
    setTextureRect(sf::IntRect(0, 0, 32, 32)); // Set the part of the texture to display
    setOrigin(0, 0);
    setPosition(-100, -100);
}

// Parametrized constructor for Bullet class, initializes bullet with position and mode
Bullet::Bullet(const sf::Vector2f& pos, const bool mode, const sf::Vector2f& direction)
    : _mode(mode), _active(true), _direction(direction) {
    setTexture(bulletTexture); // Set the texture for the bullet
    setTextureRect(sf::IntRect(0, 0, 16, 16)); // Set the part of the texture to display
    setPosition(pos);        // Set the initial position of the bullet
}

// Update method for Bullet class, called every frame to update bullet states
void Bullet::Update(const float& dt) {
    for (auto& bullet : bullets) { // Loop through each bullet
        if (bullet.isActive()) {
            bullet.move(bullet._direction * dt * bulletSpeed); // bulletSpeed is a constant for bullet speed

            if (bullet.isActive()) {   // If the bullet is active
                // Determine bullet movement direction based on its mode
                float direction = bullet.getMode() ? 1.0f : -1.0f;


                // Check if the bullet is off-screen and deactivate it
                sf::Vector2f bulletPos = bullet.getPosition();
                if (bulletPos.x < 0 || bulletPos.x > gameWidth || bulletPos.y < 0 || bulletPos.y > gameHeight) {
                    bullet.deactivate();
                    // std::cout << "Bullet deactivated as it moved off-screen." << std::endl;
                }

                else {
                    // If on-screen, check for collisions
                    bullet.checkCollisions();
                }
            }
        }
    }
}

// Getter method for the _mode member
 bool Bullet::getMode() const {
    return _mode;
}


// Render method to draw active bullets on the window
void Bullet::Render(RenderWindow& window) {
    for (const auto& bullet : bullets) {
        if (bullet.isActive()) {
            //std::cout << "Rendering active bullet at position: " << bullet.getPosition().x << ", " << bullet.getPosition().y << std::endl;

            window.draw(bullet); // Draw each active bullet
        }
    }
}

// Static method to initialize bullets
void Bullet::Init() {
    cout << "Initializing bullets..." << endl;
    for (auto& bullet : bullets) {
        bullet.deactivate(); // Deactivate all bullets initially
    }
    //cout << "Bullet initialization completed." << endl;
}

// Static method to fire a bullet
void Bullet::Fire(const sf::Vector2f& pos, const bool mode, const sf::Vector2f& direction, bool isPlayerBullet) {
    for (unsigned char i = 0; i < 64; ++i) {
        Bullet& bullet = bullets[bulletPointer];
        bulletPointer = (bulletPointer + 1) % 64; // Cycle through the bullets

        if (!bullet.isActive()) {
            bullet.activate(pos, mode, direction, isPlayerBullet); // Reactivate the bullet
            return; // Exit the method after firing a bullet
        }
    }
} 

    

// Method to check if the bullet is active
bool Bullet::isActive() const {
    return _active;
}


void Bullet::checkCollisions() {
    // Get the bullet's current position
    sf::Vector2f bulletPosition = getPosition();

    // Calculate the tile position for the bullet
    sf::Vector2ul tilePos(static_cast<unsigned>(bulletPosition.x / LevelSystem::getTileSize()),
        static_cast<unsigned>(bulletPosition.y / LevelSystem::getTileSize()));

    // Check if the tile is a wall
    if (LevelSystem::getTile(tilePos) == LevelSystem::WALL) {
        deactivate(); // Deactivate the bullet if it hits a wall
        return; // Return early as the bullet is no longer active
    }

    // Position-based collision detection with slimes
    const float collisionThreshold = 20.0f; // Define the threshold for considering a collision

    for (Slime& slime : *_slimes) { // Loop through each slime
        sf::Vector2f slimePosition = slime.getPosition();

        if (std::abs(bulletPosition.x - slimePosition.x) <= collisionThreshold &&
            std::abs(bulletPosition.y - slimePosition.y) <= collisionThreshold) {

            std::cout << "enemy has been hit" << std::endl;

            // A collision is detected
            slime.onBulletHit( _isPlayerBullet);

            if(_isPlayerBullet){
            deactivate(); // Deactivate the bullet
            return; // Return early as the bullet has collided and is no longer active
            }
        }
    }

    if (_mage != nullptr) {
        sf::Vector2f magePosition = _mage->getPosition();

        if (std::abs(bulletPosition.x - magePosition.x) <= collisionThreshold &&
            std::abs(bulletPosition.y - magePosition.y) <= collisionThreshold) {

            std::cout << "player has been hit" << std::endl;

            // Collision with Mage detected
            _mage->onBulletHit(_isPlayerBullet); // Method to handle the mage getting hit

            if (!_isPlayerBullet) {
                deactivate(); // Deactivate the bullet
                return; // Return early as the bullet has collided and is no longer active
            }
        }
    }

}


// Method to activate a bullet
void Bullet::activate(const sf::Vector2f& pos, bool mode, const sf::Vector2f& direction, bool isPlayerBullet) {
    setPosition(pos);
    _mode = mode;
    _active = true;
    _direction = direction;
    _isPlayerBullet = isPlayerBullet;

    float angle = atan2(direction.y, direction.x) * 180 / 3.141;
    setRotation(angle);
    
}

// Method to deactivate a bullet
void Bullet::deactivate() {
    setPosition(-100, -100); // Move the bullet off-screen
    _active = false;         // Mark the bullet as inactive
}

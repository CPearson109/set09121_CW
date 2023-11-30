// Include the necessary headers from SFML, and standard input-output library
#include <SFML/Graphics.hpp>
#include <iostream>

// Include custom header files for Bullet, Game, and Entity classes
#include "Bullet.h"
#include "Game.h"
#include "Entity.h"

// Use the namespaces from SFML and standard library for convenience
using namespace sf;
using namespace std;

// Static variables for the Bullet class
unsigned char Bullet::bulletPointer = 0; // A static pointer to keep track of bullets
Bullet Bullet::bullets[256];             // Static array to store bullets
std::vector<Entity*> entities;            // Global vector to store entities


// Default constructor for Bullet class, initializes a bullet as inactive and off-screen
Bullet::Bullet() : _active(false), _mode(false) {
    setTexture(bulletTexture); // Ensure texture is set
    setTextureRect(sf::IntRect(0, 0, 32, 32)); // Set the part of the texture to display
    setPosition(-100, -100);
}

// Parametrized constructor for Bullet class, initializes bullet with position and mode
Bullet::Bullet(const Vector2f& pos, const bool mode) : _mode(mode), _active(true) {
    setTexture(bulletTexture); // Set the texture for the bullet
    setTextureRect(sf::IntRect(0, 0, 32, 32)); // Set the part of the texture to display
    setPosition(pos);        // Set the initial position of the bullet
}

// Update method for Bullet class, called every frame to update bullet states
void Bullet::Update(const float& dt) {
    for (auto& bullet : bullets) { // Loop through each bullet
        if (bullet.isActive()) {   // If the bullet is active
            // Determine bullet movement direction based on its mode
            float direction = bullet.getMode() ? 1.0f : -1.0f;
            // Move the bullet vertically
            bullet.move(0, dt * 200.0f * direction);

            std::cout << "Updating active bullet. New position: " << bullet.getPosition().x << ", " << bullet.getPosition().y << std::endl;


            // Check if the bullet is off-screen and deactivate it
            if (bullet.getPosition().y < -32 || bullet.getPosition().y > gameHeight + 32) {
                bullet.deactivate();
                std::cout << "Bullet deactivated as it moved off-screen." << std::endl;

            }
            else {
                // If on-screen, check for collisions
                bullet.checkCollisions();
            }
        }
    }
}

// Getter method for the _mode member
bool Bullet::getMode() const {
    return _mode;
}

// Method to check for collisions (to be implemented)
void Bullet::checkCollisions() {
    // Implement collision logic here
}

// Render method to draw active bullets on the window
void Bullet::Render(RenderWindow& window) {
    for (const auto& bullet : bullets) {
        if (bullet.isActive()) {
            std::cout << "Rendering active bullet at position: " << bullet.getPosition().x << ", " << bullet.getPosition().y << std::endl;

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
    cout << "Bullet initialization completed." << endl;
}

// Static method to fire a bullet
void Bullet::Fire(const Vector2f& pos, const bool mode) {
    // Debug statement to indicate this method is called
    std::cout << "Bullet::Fire called at position (" << pos.x << ", " << pos.y << ") with mode " << mode << std::endl;


    Bullet& bullet = bullets[bulletPointer]; // Get a reference to the current bullet
    if (!bullet.isActive()) {                // If the bullet is not active
        bullet.activate(pos, mode);  
       
        std::cout << "Bullet activated at position: " << pos.x << ", " << pos.y << std::endl;

        // Set the initial position for the bullet
        bullet.setPosition(pos); // Set the initial position for the bullet
    }
    std::cout << "Bullet fired. Position: " << pos.x << ", " << pos.y << " | Mode: " << mode << std::endl;

    // Increment the bullet pointer in a circular manner
    bulletPointer = (bulletPointer + 1) % 256;
}

// Method to check if the bullet is active
bool Bullet::isActive() const {
    return _active;
}

// Custom method to check collisions for a specific bullet
void Bullet::checkCollisions(Bullet& bullet) {
    FloatRect boundingBox = bullet.getGlobalBounds(); // Get bullet's bounding box
    for (auto* entity : entities) {
        // Skip certain entities based on bullet's mode and entity type
        if ((bullet._mode && entity != playerMage) || (!bullet._mode && entity == playerMage)) {
            continue;
        }

        // Skip exploded entities
        if (entity->is_exploded()) {
            continue;
        }

        // Check for intersection with bullet and handle explosion
        if (entity->getGlobalBounds().intersects(boundingBox)) {
            entity->Explode();
            bullet.deactivate();
            break;
        }
    }
}

// Method to activate a bullet
void Bullet::activate(const Vector2f& pos, bool mode) {
    setPosition(pos);  // Set the position of the bullet
    _mode = mode;      // Set the mode
    _active = true;    // Mark the bullet as active
    std::cout << "Activating bullet. Position: " << pos.x << ", " << pos.y << " | Mode: " << mode << std::endl;

}

// Method to deactivate a bullet
void Bullet::deactivate() {
    setPosition(-100, -100); // Move the bullet off-screen
    _active = false;         // Mark the bullet as inactive
}

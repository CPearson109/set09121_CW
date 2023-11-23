#include "bullet.h"
#include "game.h"
#include "ship.h"
using namespace sf;
using namespace std;


unsigned char Bullet::bulletPointer = 0;
Bullet Bullet::bullets[256];

// Default constructor (might be private if you only create bullets through Fire method)
Bullet::Bullet() {
    // You might want to set default values here
}

Bullet::Bullet(const sf::Vector2f& pos, const bool mode) : _mode(mode) {
    setTexture(spritesheet);
    setPosition(pos);

}

void Bullet::Update(const float& dt) {
    for (unsigned char i = 0; i < bulletPointer; ++i) {
        if (bullets[i].isActive()) {
            // Move each active bullet
            bullets[i].move(Vector2f(0, dt * 200.0f * (bullets[i]._mode ? 1.0f : -1.0f)));

            // Check if the bullet is off-screen
            if (bullets[i].getPosition().y < -32 || bullets[i].getPosition().y > gameHeight + 32) {
                // Deactivate off-screen bullet
                bullets[i]._active = false; // Assuming _active is a member that tracks if a bullet is active
            }
            else {
                // Check for collision with ships
                const FloatRect boundingBox = bullets[i].getGlobalBounds();
                for (auto s : ships) {
                    // Make sure 'player' is a defined pointer to the player's ship
                    if (!bullets[i]._mode && s == player) {
                        // Player bullets don't collide with the player
                        continue;
                    }
                    if (bullets[i]._mode && s != player) {
                        // Invader bullets don't collide with other invaders
                        continue;
                    }
                    if (!s->is_exploded() && s->getGlobalBounds().intersects(boundingBox)) {
                        // Explode the ship
                        s->Explode();
                        // Warp bullet off-screen
                        bullets[i].setPosition(Vector2f(-100, -100));
                        bullets[i]._active = false; // Deactivate the bullet
                        break; // Exit the loop as the bullet has hit a ship
                    }
                }
            }
        }
    }
}

// Static method to render all bullets in the pool
void Bullet::Render(sf::RenderWindow& window) {
    for (unsigned char i = 0; i < bulletPointer; ++i) {
        if (bullets[i].isActive()) {
            window.draw(bullets[i]);
        }
    }
}
// Static method to initialize the bullets (e.g., during game setup)
void Bullet::Init() {
    for (unsigned char i = 0; i < 256; ++i) {
        bullets[i] = Bullet(); // Calls the default constructor
        bullets[i].setPosition(-100, -100); // Off-screen or inactive position
    }
}
void Bullet::Fire(const sf::Vector2f& pos, const bool mode) {
    // Here you would need to implement logic to find an inactive bullet and activate it.
    // // Check if bulletPointer exceeds 255 (maximum index), and wrap it back to 0 if needed
    if (bulletPointer >= 255) {
        bulletPointer = 0;
    }
    // For example:
    bullets[bulletPointer].setPosition(pos);
    bullets[bulletPointer]._mode = mode;
    // Set the bullet to active
    bulletPointer++; // Move to the next bullet in the pool
    // Note: you should also include logic to wrap the bulletPointer around to 0 if it exceeds 255
}
// You would also need to implement a method to determine if a bullet is active
bool Bullet::isActive() const {
    // This method should check if the bullet is currently active.
    // You might use position, a boolean flag, or some other mechanism to determine this.
    return getPosition().x >= 0 && getPosition().y >= 0 && _active;

}
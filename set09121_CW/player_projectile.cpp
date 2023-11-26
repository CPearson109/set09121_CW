//player_projectile.cpp
#include "player_projectile.h"

Projectile::Projectile(const sf::Vector2f& pos, const sf::Vector2f& direction, float speed)
    : _direction(direction), _speed(speed), _lifetime(5.0f) { // Default lifetime 5 seconds
    setPosition(pos);
    // Set up other properties like shape, size, color, etc.
}

void Projectile::update(double dt) {
    sf::Vector2f pos = getPosition();
    pos += _direction * _speed * dt;
    setPosition(pos);

    _lifetime -= dt;
    if (_lifetime <= 0) {
        // Mark for deletion or deactivate
    }
}

void Projectile::setLifetime(float lifetime) {
    _lifetime = lifetime;
}

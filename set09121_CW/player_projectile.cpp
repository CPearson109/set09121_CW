//player_projectile.cpp
#include "player_projectile.h"
#include "player.h"

Projectile::Projectile(Scene* scene, const sf::Vector2f& pos, const sf::Vector2f& direction, float speed, sf::Texture& texture)
    : Entity(scene), _direction(direction), _speed(speed), _lifetime(5.0f), _projectileTexture(texture) {
    setPosition(pos);
    std::cerr << "Projectile created at: " << pos.x << ", " << pos.y << std::endl;

    auto shape = addComponent<ShapeComponent>();
    shape->setShape<sf::CircleShape>(15.f);
    shape->getShape().setFillColor(sf::Color::Red);
    shape->getShape().setOrigin(15.f, 15.f); // Center origin
}

void Projectile::update(double dt) {
    sf::Vector2f pos = getPosition();
    std::cerr << "Projectile position before update: " << pos.x << ", " << pos.y << std::endl;

    pos.x += _direction.x * _speed * dt;
    pos.y += _direction.y * _speed * dt;
    setPosition(pos);
    std::cerr << "Projectile position after update: " << pos.x << ", " << pos.y << std::endl;

    _lifetime -= dt;
    if (_lifetime <= 0) {
        // Mark for deletion or deactivate
    }
}

void Projectile::setLifetime(float lifetime) {
    _lifetime = lifetime;
}

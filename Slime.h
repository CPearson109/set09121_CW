//Slime.h
#pragma once
#include "Entity.h"
#include "Mage.h"

class Slime : public Entity {
public:
    Slime(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture, Entity& player, float speed, const EntityStats& stats);
    void Update(const float& dt, sf::RenderWindow& window) override;
    void FireBullet(bool mode, const sf::Vector2f& direction, const sf::Vector2f& pos, bool isPlayerBullet);
    void onBulletHit(bool isPlayerBullet);
    bool isActive() const { return _isActive; }
    void setActive(bool active) {
        // Logic to set the active state of the slime
        _isActive = active;
    }

private:
    Animation* currentAnimation; // Declare currentAnimation as a pointer to Animation
    Animation idleAnimation;
    Animation attackAnimation;
    Animation movingAnimation;
    float _speed;
    Entity& _player;
    float _shootTimer;
    int _health;
    EntityStats _entityStats;
    bool _isActive = true;


};
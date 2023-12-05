//Mage.h
#pragma once
#include "Entity.h"
#include "Slime.h"

class Mage : public Entity {
public:

    Mage(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture, const EntityStats& stats); // Add EntityStats parameter
    void  Update(const float& dt, sf::RenderWindow& window) override;
    void FireBullet(bool mode, const sf::Vector2f& direction, const sf::Vector2f& pos);
    void onBulletHit(bool isPlayerBullet);

    // Method to get the current stats of the Mage
    EntityStats getStats() const {
        return _entityStats;
    }

    // Method to set the stats of the Mage
    void setStats(const EntityStats& newStats) {
        _entityStats = newStats;
    }

    bool isPlayerDead() const;

    void resetPlayer();

private:
    Animation walkDownAnimation;
    Animation walkUpAnimation;
    Animation walkLeftAnimation;
    Animation walkRightAnimation;

    // Enumeration for directions
    enum class Direction { Up, Down, Left, Right };
    Direction currentDirection = Direction::Down; // Default direction

    //variable to store stats
    EntityStats _entityStats;

    // Speed of the mage
    float speed;
    float _shootTimer;
    EntityStats _stats;
    bool _isPlayerDead = false;
};
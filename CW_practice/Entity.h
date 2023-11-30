//Entity.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"


class Entity : public sf::Sprite {
protected:
    sf::IntRect _sprite;
    //sf::Vector2f pos;
    bool _exploded = false;
    //Default constructor is hidden
    Entity();
public:
    virtual bool is_exploded() const;
    virtual void Explode();
    static bool direction;
    static float speed;
    //Constructor that takes a sprite
    explicit Entity(sf::IntRect ir, sf::Vector2f pos);
    //Pure virtual deconstructor -- makes this an abstract class and avoids undefined behaviour!
    virtual ~Entity() = 0;
    //Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float& dt);
};
class Mage : public Entity {
public:
    Mage(); // Default constructor declaration
    Mage(sf::IntRect ir, sf::Vector2f pos);
    void Update(const float& dt) override;
    void FireBullet(bool mode);

private:
    Animation walkDownAnimation;
    Animation walkUpAnimation;
    Animation walkLeftAnimation;
    Animation walkRightAnimation;

    // Enumeration for directions
    enum class Direction { Up, Down, Left, Right };
    Direction currentDirection = Direction::Down; // Default direction

    // Speed of the mage
    float speed;
};


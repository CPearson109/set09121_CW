//ship.h
#pragma once
#include <SFML/Graphics.hpp>


class Ship : public sf::Sprite {
protected:
    sf::IntRect _sprite;
    //sf::Vector2f pos;
    bool _exploded = false;
    //Default constructor is hidden
    Ship();
public:
    virtual bool is_exploded() const;
    virtual void Explode();
    static bool direction;
    static float speed;
    //Constructor that takes a sprite
    explicit Ship(sf::IntRect ir, sf::Vector2f pos);
    //Pure virtual deconstructor -- makes this an abstract class and avoids undefined behaviour!
    virtual ~Ship() = 0;
    //Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float& dt);
};
//class Invader : public Ship {
//public:
//    Invader(sf::IntRect ir, sf::Vector2f pos);
//    Invader();
//    void Update(const float& dt) override;
//    bool is_exploded() const;
//    virtual void Explode();
//};
//
//class Player : public Ship {
//public:
//    Player();
//    void Update(const float& dt) override;
//};
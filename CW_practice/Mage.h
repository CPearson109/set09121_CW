// Including necessary headers
#include <iostream>
#include "Ship.h"
#include "Animation.h"

// Defining the Mage class, which inherits from Ship
class Mage : public Ship {
public:
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

// Implementation of the constructor
//Mage::Mage(sf::IntRect ir, sf::Vector2f pos)
//    : Ship(ir, pos) {
//    // Initialize other properties of the Mage
//    // ...
//}
//
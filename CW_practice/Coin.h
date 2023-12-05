#pragma once
#include "Entity.h"
#include "Animation.h"

class Coin : public Entity {
public:
    Coin(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture);

    void Update(const float& dt, sf::RenderWindow& window) override;

private:
    Animation spinAnimation;
};

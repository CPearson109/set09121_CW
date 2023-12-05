#include "Coin.h"

Coin::Coin(sf::IntRect ir, sf::Vector2f pos, sf::Texture & texture)
    : Entity(ir, pos, texture),

    spinAnimation(0, 0, 50, 50, 6, 0.1f) // Assuming each frame is 32x32 pixels
{
    this->setTexture(texture); 
    spinAnimation.ApplyToSprite(*this);
}

void Coin::Update(const float& dt, sf::RenderWindow& window) {
    spinAnimation.Update(dt);
    spinAnimation.ApplyToSprite(*this);
}
void updateCoinText(sf::Text& coinText, int score) {
    coinText.setString("Coins: " + std::to_string(score));
}

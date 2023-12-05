// game.h
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


class Entity;
class Bullet; // forward-declare Bullet
class Mage;   // Forward-declare Mage

constexpr uint16_t gameWidth = 1400;
constexpr uint16_t gameHeight = 900;

extern sf::Texture spritesheet;
extern std::vector<Bullet*> bullets;
extern sf::Texture bulletTexture;

extern Mage* playerMage; // Declare playerMage


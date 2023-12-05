// Entity.cpp
#include "Entity.h" 
#include "Game.h"
#include "Bullet.h"
#include "LevelSystem.h"
#include "EntityStats.h"


using namespace sf;
using namespace std;


Entity::Entity() {};

//constructor with an initialization list
Entity::Entity(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture) : _sprite(), _texture(texture) {
    _sprite = ir;
    setTexture(spritesheet);
    setTextureRect(_sprite);
    setPosition(pos);
    // Initialize other member variables if needed
}
const sf::Vector2f& Entity::getPosition() const {
    return _position;
}

void Entity::setPosition(const sf::Vector2f& position) {
    _position = position;
    sf::Sprite::setPosition(position);
}

void Entity::Update(const float& dt, sf::RenderWindow& window) {}

Entity::~Entity() {}







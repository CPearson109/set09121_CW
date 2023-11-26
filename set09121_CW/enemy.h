//Enemy.h
#pragma once
#include "SFML/Graphics.hpp"
#include "ecm.h"
#include "cmp_sprite.h"

class Enemy : public Entity {
private:
	float _speed;

public:
	void Update(double dt) ;
	Enemy();
	void Render(sf::RenderWindow& window) const ;
};
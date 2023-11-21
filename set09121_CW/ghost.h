//ghost.h
#pragma once
#include "SFML/Graphics.hpp"
#include "ecm.h"
#include "cmp_sprite.h"

class Ghost : public Entity {
private:
	float _speed;

public:
	void Update(double dt) ;
	Ghost();
	void Render(sf::RenderWindow& window) const ;
};
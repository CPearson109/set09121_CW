//bullet.h
#pragma once
#include <SFML/Graphics.hpp>
#include "game.h"

class Bullet : public sf::Sprite {
public:
	Bullet(); 
	Bullet(const sf::Vector2f& pos, bool mode);
	//updates all bullets (by calling _Update() on all bullets in the pool)
	static void Update(const float& dt);
	//Render's all bullets (uses a similar trick to the ship renderer but on the bullet pool)
	static void Render(sf::RenderWindow& window);
	//Chose an inactive bullet and use it.
	static void Fire(const sf::Vector2f& pos, const bool mode);
	//Set all the bullets to -100, -100, set the spritesheet, set origin
	static void Init();

	
	bool isActive() const; // Declare isActive method

	~Bullet() = default;
	void activate(const sf::Vector2f& pos, bool mode);
	void deactivate();
	bool getMode() const;

private:
	// ... other members ...

	void checkCollisions();

	// ... other members ...


protected:
	static unsigned char bulletPointer;
	static Bullet bullets[256];
	bool _active; // You might add an _active member to track the bullet's state
	
	//false=player bullet, true=Enemy bullet
	bool _mode;
	void checkCollisions(Bullet& bullet); // private member function for collision checking
};


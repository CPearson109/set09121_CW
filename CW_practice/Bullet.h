//bullet.h
#pragma once
#include <SFML/Graphics.hpp>
#include "game.h"
#include "Entity.h"

class Bullet : public sf::Sprite {
public:
	Bullet(); 
	Bullet(const sf::Vector2f& pos, const bool mode, const sf::Vector2f& direction);
	//updates all bullets (by calling _Update() on all bullets in the pool)
	static void Update(const float& dt);
	//Render's all bullets (uses a similar trick to the ship renderer but on the bullet pool)
	static void Render(sf::RenderWindow& window);
	//Chose an inactive bullet and use it.
	static void Fire(const sf::Vector2f& pos, bool mode, const sf::Vector2f& direction);

	//Set all the bullets to -100, -100, set the spritesheet, set origin
	static void Init();

	
	bool isActive() const; // Declare isActive method

	~Bullet() = default;
	void activate(const sf::Vector2f& pos, bool mode, const sf::Vector2f& direction);

	void deactivate();
	bool getMode() const;
	static const float bulletSpeed;  // Declaration

	static void setSlimesReference(std::vector<Slime>& slimesRef) {
		_slimes = &slimesRef;
	}

private:
	// ... other members ...
	sf::Vector2f _direction; 
	void checkCollisions();

	static std::vector<Slime>* _slimes;



protected:
	static unsigned char bulletPointer;
	static Bullet bullets[64];
	bool _active; // You might add an _active member to track the bullet's state
	
	//false=player bullet, true=Enemy bullet
	bool _mode;
	void checkCollisions(Bullet& bullet); // private member function for collision checking
};


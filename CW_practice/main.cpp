#include <SFML/Graphics.hpp>
#include <iostream>
#include "Ship.h"
#include "Game.h"
#include "Bullet.h"
#include "LevelSystem.h"
#include "Mage.h"
#include "PauseMenu.h"


using namespace std;
using namespace sf;

std::vector<Ship*> ships;
sf::Texture spritesheet;
sf::Texture bulletTexture; // Declare a texture for the bullet
sf::Sprite invader;
Ship* player = nullptr;
std::vector<Bullet*> bullets;
PauseMenu pauseMenu;

// Function to initialize the bullet pool
void InitializeBulletPool() {
    Bullet::Init();
}


void Load() {
    if (!spritesheet.loadFromFile("C:/Users/romeo/year1_game/res/img/mage.png")) {
        std::cerr << "Failed to load spritesheet." << std::endl;
        // Handle the error, e.g., exit the program
    }
    // Load the texture for the bullet
    if (!bulletTexture.loadFromFile("C:/Users/romeo/year1_game/res/img/Fireball-1.png")) {
        std::cerr << "Failed to load bullet texture." << std::endl;
        // Handle the error, e.g., exit the program
    }
   
}


int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 900), "Wizards of the Abyss");
    Load();

    // Initialize the bullet pool here
    //InitializeBulletPool();



      // Load the background image for the pause menu
    sf::Texture pauseMenuBackgroundTexture;
    if (!pauseMenuBackgroundTexture.loadFromFile("C:/Users/romeo/year1_game/res/img/book_pages.png")) {
        std::cerr << "Failed to load pause menu background image." << std::endl;
        return 1;
    }

    // Create an instance of the PauseMenu and pass the background texture
    PauseMenu pauseMenu(pauseMenuBackgroundTexture);


    LevelSystem::loadLevelFile("C:\\Users\\romeo\\year1_game\\res\\levels\\level_3.txt", 35.f); // You'll need to provide the correct path
    
    // Get the start position from the level system
    sf::Vector2f startPos = LevelSystem::getStartTilePosition();
    if (startPos.x < 0 || startPos.y < 0) {
        std::cerr << "Start position not found in the level." << std::endl;
        return 1;
    }

    //create a Mage instance
        Mage myMage(sf::IntRect(0, 0, 35, 37), startPos); 


    // Load the background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:/Users/romeo/year1_game/res/img/3rd_map.png")) {
        std::cerr << "Failed to load background image." << std::endl;
        return 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        myMage.FireBullet(false); // Assuming false means it's a player bullet
    }

    // Declare an sf::Clock object
    sf::Clock clock; 


    sf::Sprite backgroundSprite(backgroundTexture);

    // Variable to track whether the spacebar was previously pressed
    bool spacebarPressed = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Check for 'P' key press to toggle the pause menu
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                pauseMenu.Toggle(); // Toggle the pause menu's open/closed state
            }
        }
       
   

        float dt = clock.restart().asSeconds();


        window.clear();

        if (!pauseMenu.IsOpen()) {
            // Update game logic when the pause menu is closed
            myMage.Update(dt);
            Bullet::Update(dt);
        }




        // Update the Mage 
        myMage.Update(dt);

        // Update all bullets
        Bullet::Update(dt); 
        // Draw the background
        window.draw(backgroundSprite);
        // Render the tilemap on top of the background
        LevelSystem::Render(window);
       





        // Check for spacebar key press and fire a bullet from the Mage
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            // Check if the spacebar was not pressed in the previous frame
            if (!spacebarPressed) {
                // Assuming false means it's a player bullet
                myMage.FireBullet(false); 
            }
            // Set the flag to true when spacebar is pressed
            spacebarPressed = true; 
        }
        else {
            // Reset the flag when spacebar is released
            spacebarPressed = false; 
        }





        // Draw the Mage
        window.draw(myMage);

        // Render active bullets
        Bullet::Render(window);







        if(!pauseMenu.IsOpen()) {
            window.draw(myMage);
            Bullet::Render(window);
        }
        else {
            // Render the pause menu
            pauseMenu.Display(window);
        }


        window.display();
    }

    return 0;
}

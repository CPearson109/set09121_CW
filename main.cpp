#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Entity.h"
#include "EntityStats.h"
#include "Game.h"
#include "Bullet.h"
#include "LevelSystem.h"
#include "PauseMenu.h"
#include "Coin.h"
#include "Slime.h"
#include "Mage.h"
#include "DeadScreen.h"


using namespace std;
using namespace sf;

sf::Texture backgroundTexture; 
sf::Sprite backgroundSprite(backgroundTexture); 
sf::Texture spritesheet;
sf::Texture mageSpritesheet; // Texture for the Mage
sf::Texture slimeSpritesheet; // Texture for the Slime
sf::Texture coinSpriteSheet; //Texture for coin
sf::Texture bulletTexture; // Texture for the bullet 
sf::Font globalFont;
sf::Text coinText;
 

Mage* playerMage = nullptr; // Initialize the static member
std::vector<Bullet*> bullets;
PauseMenu pauseMenu;
int currentLevel = 1;

std::vector<Slime> slimes;

enum class GameState {
    Playing,
    Paused
};


// Function to initialize the bullet pool
void InitializeBulletPool() {
    std::cout << "Initializing Bullet Pool..." << std::endl; // Debug statement
    Bullet::Init();
    std::cout << "Bullet Pool Initialized." << std::endl; // Debug statement
}

void ClearLevel(std::vector<Slime>& slimes) {
    // Clear all slimes
    slimes.clear();
}


void Load(int level, sf::Texture& backgroundTexture, sf::Sprite& backgroundSprite, std::vector<Slime>& slimes, Mage& myMage) {

        ClearLevel(slimes);

    // Load the Mage spritesheet
    if (!mageSpritesheet.loadFromFile("D:/set09121_CW-main/res/img/mage.png")) {
        std::cerr << "Failed to load Mage spritesheet." << std::endl;
    }
    else {
        std::cout << "Mage spritesheet loaded successfully." << std::endl;
    }

    // Load the Slime spritesheet

    if (!slimeSpritesheet.loadFromFile("D:/set09121_CW-main/res/img/slime_brown.png")) {
        std::cerr << "Failed to load Slime spritesheet." << std::endl;
    }
    else {
        std::cout << "Slime spritesheet loaded successfully." << std::endl;
    }

    // Load the bullet texture using bulletTexture
    if (!bulletTexture.loadFromFile("D:/set09121_CW-main/res/img/Fireball-1.png")) {

        std::cerr << "Failed to load bullet texture." << std::endl;
    }
    else {
        std::cout << "Bullet texture loaded successfully." << std::endl;
    }
    

    if (!coinSpriteSheet.loadFromFile("D:/set09121_CW-main/res/img/Gold_Coin.png")) {
        std::cerr << "Failed to load coin sprite sheet." << std::endl;
    }
    else {
        std::cout << "Coin spritesheet loaded successfully." << std::endl;
    }


    //tilemap
    string levelPath;
    if (level == 1) {
        levelPath = "D:/set09121_CW-main/res/levels/level_1.txt";
    }
    else if (level == 2) {
        levelPath = "D:/set09121_CW-main/res/levels/level_2.txt";
    }
    else if (level == 3) {
        levelPath = "D:/set09121_CW-main/res/levels/level_3.txt";
    }

    // Reactivate all slimes
    for (auto& slime : slimes) {
        slime.setActive(true); // Assuming there's a setActive method
    }

    // Background image paths
    string backgroundImagePath; 
    if (level == 1) { 
        backgroundImagePath = "D:/set09121_CW-main/res/img/1st_map.png"; // Corrected
    }
    else if (level == 2) { 
        backgroundImagePath = "D:/set09121_CW-main/res/img/2nd_map.png";
    }
    else if (level == 3) { 
        backgroundImagePath = "D:/set09121_CW-main/res/img/3rd_map.png";
    }

    // Load the background image
    if (!backgroundTexture.loadFromFile(backgroundImagePath)) {
        std::cerr << "Failed to load background image for level " << level << "." << std::endl;
        // Handle the error, possibly by returning false or exiting
    }
    else {
    // Set the texture to the background sprite
    backgroundSprite.setTexture(backgroundTexture); 
    backgroundSprite.setTextureRect(sf::IntRect(0, 0, 1400, 900)); // Adjust as per your window size
    }

    LevelSystem::loadLevelFile(levelPath, 35.f);

    std::vector<sf::Vector2f> slimePositions = LevelSystem::getAllStartTilePositions();
    EntityStats slimeStats(100, 100.f, 1.0f, 10); //adjust these values as needed
    for (const auto& pos : slimePositions) {
        slimes.emplace_back(sf::IntRect(0, 0, 49, 62), pos, slimeSpritesheet, myMage, 50.f, slimeStats);
        slimes.back().setTexture(slimeSpritesheet);
    }

}




int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 900), "Wizards of the Abyss");

    // Initialize currentLevel to 1
    currentLevel = 1;


    // Initialize the bullet pool here
   InitializeBulletPool();

   //sf::Music backgroundMusic;
  //// Load and play background music
  //if (!backgroundMusic.openFromFile("D:/set09121_CW-main/res/audio/relicario.wav")) {
  //    std::cerr << "Failed to load background music." << std::endl; 
  //    return 1; // or handle the error as needed
  //}
  //backgroundMusic.setLoop(true); // Set the music to loop 
  //backgroundMusic.setVolume(50); // Set the volume (0 to 100) 
  //backgroundMusic.play(); // Start playing the music 



     // Initialize the game state to Playing
    GameState gameState = GameState::Playing;

    int score = 100; // Initialize score variable

    // Load the background image for the pause menu
    sf::Texture pauseMenuBackgroundTexture;
    if (!pauseMenuBackgroundTexture.loadFromFile("D:/set09121_CW-main/res/img/book_pages.png")) {

        std::cerr << "Failed to load pause menu background image." << std::endl;
        return 1;
    }

    //font laoding
    if (!globalFont.loadFromFile("D:/set09121_CW-main/res/fonts/Roboto-Black.ttf")) { 
        std::cerr << "Failed to load font." << std::endl;  
        return 1; // Or handle the error as needed 
    }
     

    //stats
    pauseMenu.setFont(globalFont);
    coinText.setFont(globalFont); 
    coinText.setString("X : " + std::to_string(score)); //string 
    coinText.setCharacterSize(24); //  size 
    coinText.setFillColor(sf::Color::Yellow); // color 
    coinText.setPosition(60, 60); // position 
    


    // Positions for the coin sprite in different states
    sf::Vector2f playingPosition(0, 0); // Top left of the screen 
    sf::Vector2f pausedPosition(10, 50); // Next to "Coins: 100" text 



    // Create a coin entity
    Coin coin(sf::IntRect(0, 0, 50, 50), playingPosition, coinSpriteSheet);  

    // Create an instance of the PauseMenu and pass the background texture
    PauseMenu pauseMenu(globalFont, pauseMenuBackgroundTexture); 

    // Declare and initialize mageStats
    EntityStats mageStats(100.f, 100.f, 1.0f, 10.f); //adjust these values as needed

    // Add upgrade options 
  // Add upgrade options with lambda functions that modify mageStats
    pauseMenu.AddUpgradeOption("Increase Player Health", 10, [&mageStats]() { 
        mageStats.increaseHealth(10); // Increase health by 10 
        });

    pauseMenu.AddUpgradeOption("Increase Player Speed", 10, [&mageStats]() { 
        mageStats.increaseMovementSpeed(5); // Increase speed by 5 
        });

    pauseMenu.AddUpgradeOption("Increase Attack Speed", 20, [&mageStats]() { 
        mageStats.increaseAttackSpeed(0.1f); // Increase attack speed 
        });

    pauseMenu.AddUpgradeOption("Increase Attack Damage", 30, [&mageStats]() { 
        mageStats.increaseAttackDamage(5); // Increase attack damage by 5 
        });



    // Initialize Mage with default position
    sf::Vector2f defaultPosition(0, 0); // Replace with an actual default position
    Mage myMage(sf::IntRect(0, 0, 35, 37), defaultPosition, mageSpritesheet, mageStats);
    myMage.setTexture(mageSpritesheet);

    // Load the current level
    Load(currentLevel, backgroundTexture, backgroundSprite, slimes, myMage);

    // Now that the level is loaded, get the start position for the Mage and update its position
    sf::Vector2f startPos = LevelSystem::getStartTilePosition();
    myMage.setPosition(startPos);

    // Get enemy positions for the current level and create Slimes
    std::vector<sf::Vector2f> enemyPositions = LevelSystem::getAllStartTilePositions();
    EntityStats slimeStats(100, 100.f, 1.0f, 10); // Adjust these values as needed
    slimes.clear();
    for (const auto& pos : enemyPositions) {
        slimes.emplace_back(sf::IntRect(0, 0, 49, 62), pos, slimeSpritesheet, myMage, 50.f, slimeStats);
        slimes.back().setTexture(slimeSpritesheet);
    }

    Bullet::setSlimesReference(slimes);

    Bullet::setMage(&myMage);

    // Declare an sf::Clock object
    sf::Clock clock;


    sf::Sprite backgroundSprite(backgroundTexture);

    // Create a sprite for the pause menu 
    sf::Sprite PauseSprite(pauseMenuBackgroundTexture);


    // Variable to track whether the spacebar was previously pressed
    bool spacebarPressed = false;
   
    // Variable to track whether the 'P' key was pressed in the previous frame
    static bool prevPKeyPressed = false;


    while (window.isOpen()) {
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        float dt = clock.restart().asSeconds();

        sf::Vector2f playerPos = myMage.getPosition();
        sf::Vector2ul tilePos(static_cast<unsigned>(playerPos.x / LevelSystem::getTileSize()),
            static_cast<unsigned>(playerPos.y / LevelSystem::getTileSize()));

        if (LevelSystem::getTile(tilePos) == LevelSystem::END) {
            currentLevel++;
            if (currentLevel > 3) {
                currentLevel = 1;
            }
            Load(currentLevel, backgroundTexture, backgroundSprite, slimes, myMage);
            sf::Vector2f startPos = LevelSystem::getStartTilePosition();
            myMage.setPosition(startPos);
        }


        // Update the score text position and string
        coinText.setString("X " + std::to_string(score)); 
        sf::FloatRect coinBounds = coin.getGlobalBounds(); 
        coinText.setPosition(coinBounds.left + coinBounds.width + 10, coinBounds.top + 10); 


        window.clear();

        // Update the coin
        coin.Update(dt, window); 

        DeadScreen deadScreen(window);

        if (myMage.isPlayerDead()) {
            if (!deadScreen.IsOpen()) {
                deadScreen.Toggle(); // Open the dead screen
            }
            deadScreen.Display(); // Display the dead screen

            if (deadScreen.isRestartRequested()) {

                currentLevel = 1;
                score = 100;
                Load(currentLevel, backgroundTexture, backgroundSprite, slimes, myMage);
                myMage.resetPlayer();
                deadScreen.Toggle();
            }
        }
        else {
            if (deadScreen.IsOpen()) {
                deadScreen.Toggle();
            }


            bool pKeyCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::P);
            if (pKeyCurrentlyPressed && !prevPKeyPressed) {
                gameState = (gameState == GameState::Playing) ? GameState::Paused : GameState::Playing;
                pauseMenu.Toggle();

                // Update coin position based on game state
                if (gameState == GameState::Paused) {
                    coin.setPosition(pausedPosition);
                }
                else {
                    coin.setPosition(playingPosition);
                }

            }
            prevPKeyPressed = pKeyCurrentlyPressed;

            // Update mageStats in myMage if there's any change
            myMage.setStats(mageStats);

            // Update the previous key state
            prevPKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::P);

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    pauseMenu.handleMouseClick(window, score);
                }
            }


            window.clear();


            // Get the current view from the window
            const sf::View& currentView = window.getView();

            // Draw gameplay elements
            window.draw(backgroundSprite);

            //Draw the coin 
            window.draw(coinText);
            window.draw(coin);


            //gamestates
            if (gameState == GameState::Playing) {
                // Place all update logic here
                pauseMenu.HandleInput(window, score); // Pass the score as reference 
                myMage.Update(dt, window);
                Bullet::Update(dt);
                for (auto& slime : slimes) {
                    if (slime.isActive()) {
                        slime.Update(dt, window);
                    }
                }

                // Get the current view from the window
                const sf::View& currentView = window.getView();

                // Draw gameplay elements
                window.draw(backgroundSprite);
                LevelSystem::Render(window);
                for (auto& slime : slimes) {
                    if (slime.isActive()) {
                        window.draw(slime);
                    }
                }
                window.draw(myMage);
                Bullet::Render(window);


            }
            else if (gameState == GameState::Paused) {
                // Draw the pause menu here 
                window.draw(PauseSprite);



                pauseMenu.SetMageStats(myMage.getStats());
                pauseMenu.Display(window);

                // Update the coin
                window.draw(coinText); // Ensure this line is present  
                window.draw(coin); // Draw the coin


            }

            window.display();
        }
    }

    return 0;
}

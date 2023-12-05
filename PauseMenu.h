#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include <SFML/Graphics.hpp>
#include "EntityStats.h"
#include <functional> // Include for std::function
#include <vector>
#include <string>

class PauseMenu {
public:
    PauseMenu(); // Default constructor

    PauseMenu(const sf::Font& font, const sf::Texture& backgroundTexture);
    
    void SetMageStats(const EntityStats& stats); //method to set stats
    void setFont(const sf::Font& font); // Declare setFont method
    void Toggle(); // Function to toggle the pause menu's open/closed state
    bool IsOpen(); // Function to check if the pause menu is open
    void HandleInput(sf::RenderWindow& window, int& playerCoins);
    void Display(sf::RenderWindow& window); // Function to display the pause menu

    void AddUpgradeOption(const std::string& name, float cost, std::function<void()> applyUpgrade);
    void handleMouseClick(sf::RenderWindow& window, int& playerCoins);
   

private:
    EntityStats mageStats; // Member to hold mage stats
    bool isOpen; // Flag to indicate if the pause menu is open
    sf::Text statsText; // To display stats
    sf::Font font; // Add this to display text
    sf::RectangleShape background; // Background rectangle for the pause menu
    sf::RectangleShape textBackground;
    sf::RectangleShape statsBackground; 
    // Assuming statsText is the sf::Text that holds the stats info
    float offsetY = statsText.getPosition().y + statsText.getGlobalBounds().height + 10; // 10 is padding
    struct UpgradeOption {
        std::string name;
        float cost;
        std::function<void()> applyUpgrade;
        sf::Text text;
        sf::RectangleShape button;
        bool purchased = false;

        UpgradeOption(const std::string& name, float cost, const sf::Font& font, float y)
            : name(name), cost(cost), purchased(false) {
            text.setFont(font);
            text.setString(name + " - Cost: " + std::to_string(static_cast<int>(cost)));
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition(100, y);

            button.setSize(sf::Vector2f(100, 30));
            button.setFillColor(sf::Color(100, 100, 250));
            button.setPosition(400, y);
        }
    };
    std::vector<UpgradeOption> upgradeOptions; // Holds all upgrade options 
    int selectedOption = 0; // Index of the currently selected option 
};

#endif // PAUSEMENU_H

#include "PauseMenu.h"
#include "Coin.h"
#include <iomanip>
#include <sstream>


PauseMenu::PauseMenu() : isOpen(false) { 
     
    background.setSize(sf::Vector2f(1400, 900)); // Set the size to match the window size 
    background.setFillColor(sf::Color(0, 0, 0, 128)); // Semi-transparent black background
    textBackground.setSize(sf::Vector2f(300, 150)); // Set size as needed 
    textBackground.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black 
    textBackground.setPosition(90, 90); // Just behind the text 
}

void PauseMenu::SetMageStats(const EntityStats& stats) {
    std::stringstream statsStream;
    statsStream << std::fixed << std::setprecision(1); // Set precision to 1 decimal place
    statsStream << "Health: " << stats.getHealth() << std::endl;
    statsStream << "Speed: " << stats.getMovementSpeed() << std::endl;
    statsStream << "Attack Speed: " << stats.getAttackSpeed() << std::endl;
    statsStream << "Damage: " << stats.getAttackDamage();

    std::string statsString = statsStream.str();
    statsText.setString(statsString);
}

void PauseMenu::setFont(const sf::Font& font) {
    statsText.setFont(font);
    // Set other properties of statsText if needed
}


PauseMenu::PauseMenu(const sf::Font& font, const sf::Texture& backgroundTexture) : isOpen(false) {
    // Constructor with a background texture
    background.setTexture(&backgroundTexture);
    background.setSize(sf::Vector2f(1400, 900)); // Set the size to match the window size

    this->font = font; // Store the font in a member variable

    statsText.setFont(this->font);
    statsText.setCharacterSize(24); 
    statsText.setFillColor(sf::Color::White); 
    statsText.setPosition(100, 100); 

}

void PauseMenu::Toggle() {
    isOpen = !isOpen; // Toggle the pause menu's open/closed state
   
}

bool PauseMenu::IsOpen() {
   
    return isOpen;
}



void PauseMenu::AddUpgradeOption(const std::string& name, float cost, std::function<void()> applyUpgrade) {
    float startY = textBackground.getPosition().y + textBackground.getGlobalBounds().height + 10 + upgradeOptions.size() * 40;
    UpgradeOption option(name, cost, font, startY);
    option.applyUpgrade = applyUpgrade; // Assign the lambda function here
    upgradeOptions.push_back(option);
}

void PauseMenu::HandleInput(sf::RenderWindow& window, int& playerCoins) {
    if (!isOpen) return;

    // Handle navigation through upgrade options
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        selectedOption = (selectedOption + upgradeOptions.size() - 1) % upgradeOptions.size();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        selectedOption = (selectedOption + 1) % upgradeOptions.size();
    }

    // Handle upgrade purchase
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        if (playerCoins >= upgradeOptions[selectedOption].cost) {
            playerCoins -= static_cast<int>(upgradeOptions[selectedOption].cost); 
            upgradeOptions[selectedOption].applyUpgrade(); 
        }
    }

    // Update option colors based on current selection
    for (size_t i = 0; i < upgradeOptions.size(); ++i) {
        upgradeOptions[i].text.setFillColor(i == selectedOption ? sf::Color::Red : sf::Color::White);
    }
}

void PauseMenu::Display(sf::RenderWindow& window) {
    if (!isOpen) return;

    window.draw(background);
    window.draw(textBackground);
    window.draw(statsText);

    float offsetY = textBackground.getPosition().y + textBackground.getGlobalBounds().height + 300;
    for (auto& option : upgradeOptions) {
        option.text.setPosition(100, offsetY);
        option.button.setPosition(400, offsetY);
        window.draw(option.text);
        window.draw(option.button);
        offsetY += 40; // Increment y position for the next option
    }
} 

void PauseMenu::handleMouseClick(sf::RenderWindow& window, int& playerCoins) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto& option : upgradeOptions) {
        if (option.button.getGlobalBounds().contains(mousePos) && !option.purchased) {
            if (playerCoins >= option.cost) {
                playerCoins -= static_cast<int>(option.cost);
                option.applyUpgrade();
                option.purchased = true;
                option.text.setFillColor(sf::Color::Green); // Indicate the upgrade has been purchased
                option.button.setFillColor(sf::Color(50, 50, 50)); // Dim the button
            }
        }
    }
}



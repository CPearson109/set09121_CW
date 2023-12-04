#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include <SFML/Graphics.hpp>

class PauseMenu {
public:
    PauseMenu(); // Default constructor
    PauseMenu(const sf::Texture& backgroundTexture); // Constructor
    void Toggle(); // Function to toggle the pause menu's open/closed state
    bool IsOpen(); // Function to check if the pause menu is open
    void Display(sf::RenderWindow& window); // Function to display the pause menu

private:
    bool isOpen; // Flag to indicate if the pause menu is open
    sf::RectangleShape background; // Background rectangle for the pause menu
    //sf::Texture backgroundTexture; // Texture for the background
};

#endif // PAUSEMENU_H

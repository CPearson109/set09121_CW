#include "PauseMenu.h"

PauseMenu::PauseMenu() : isOpen(false) {
 
    background.setSize(sf::Vector2f(1400, 900)); // Set the size to match the window size
    background.setFillColor(sf::Color(0, 0, 0, 128)); // Semi-transparent black background
}

PauseMenu::PauseMenu(const sf::Texture& backgroundTexture) : isOpen(false) {
    // Constructor with a background texture
    background.setTexture(&backgroundTexture);
    background.setSize(sf::Vector2f(1400, 900)); // Set the size to match the window size
}

void PauseMenu::Toggle() {
    isOpen = !isOpen; // Toggle the pause menu's open/closed state
}

bool PauseMenu::IsOpen() {
    return isOpen;
}

void PauseMenu::Display(sf::RenderWindow& window) {
    if (isOpen) {
        window.draw(background);
        // You can add text or buttons for resuming the game here
    }
}

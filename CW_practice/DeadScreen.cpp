// DeadScreen.cpp
#include "DeadScreen.h"

DeadScreen::DeadScreen(sf::RenderWindow& window)
    : _window(window), _restartRequested(false), isOpen(false) {

    if (!_font.loadFromFile("D:/set09121_CW-main/res/fonts/Roboto-Black.ttf")) {
        // Handle error
    }

    // Set up the text
    _text.setFont(_font);
    _text.setString("You Died! Press R to Restart");
    _text.setCharacterSize(30);
    _text.setFillColor(sf::Color::Red);
    sf::FloatRect textRect = _text.getLocalBounds();
    _text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    _text.setPosition(_window.getSize().x / 2.0f, _window.getSize().y / 2.0f);
}

void DeadScreen::Toggle() {
    isOpen = !isOpen;
    if (!isOpen) {
        _restartRequested = false; // Reset restart flag when closing
    }
}

bool DeadScreen::IsOpen() {
    return isOpen;
}

void DeadScreen::Display() {
    if (!isOpen) return;

    _window.clear();  // Clear the window to draw only the dead screen
    _window.draw(_text);
    _window.display();

    // Check for restart
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        _restartRequested = true;
    }
}

bool DeadScreen::isRestartRequested() const {
    return _restartRequested;
}

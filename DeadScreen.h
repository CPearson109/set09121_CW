#pragma once
#include <SFML/Graphics.hpp>

class DeadScreen {
public:
    DeadScreen(sf::RenderWindow& window);

    bool isRestartRequested() const;
    void Toggle();
    bool IsOpen();
    void Display();

private:
    sf::RenderWindow& _window;
    sf::Text _text;
    sf::Font _font;
    bool _restartRequested;
    sf::RectangleShape background;
    sf::Text messageText;
    bool isOpen;
};

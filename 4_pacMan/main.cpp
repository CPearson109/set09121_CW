//main.cpp
#include <iostream>
#include "player.h"
#include "ghost.h"
#include "pacman.h"
#include "system_renderer.h"
#include "scene.h"
#include "ecm.h"

using namespace sf;
using namespace std;

const int gameWidth = 700;
const int gameHeight = 900;


void Load() {
    gameScene.reset(new GameScene());
    menuScene.reset(new MenuScene());
    gameScene->load();
    menuScene->load();
    // Start at main menu
    activeScene = menuScene;
}

void Update() {
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();
    activeScene->update(dt);
}

void Render(RenderWindow& window) {
    activeScene->render();
    // flush to screen
    Renderer::render();
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PacMan");
    Renderer::initialise(window);
    Load();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        window.clear();
        Update();
        Render(window);
        window.display();
    }

    return 0;
}

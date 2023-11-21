// pacman.cpp
#include <SFML/Graphics.hpp>
#include "player.h"
#include "ghost.h"
#include "pacman.h"
#include "system_renderer.h"
#include "scene.h"
#include "ecm.h"
#include "cmp_actor_movement.h"
#include "PlayerMovementComponent.h"
#include "LevelSystem.h"
#include "cmp_enemy_ai.h"


#define GHOSTS_COUNT 4

std::vector<std::shared_ptr<Entity>>& Scene::getEnts() {
    return _ents.list;
}

std::shared_ptr<Scene> gameScene;
std::shared_ptr<Scene> menuScene;
std::shared_ptr<Scene> activeScene;

// Menu Scene
void MenuScene::update(double dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        activeScene = gameScene;
    }
    Scene::update(dt);
    text.setString("Almost Pacman");
}

void MenuScene::render() {
    Renderer::queue(&text);
    Scene::render();
}

void MenuScene::load() {
    // Load and set up the text element
    if (!font.loadFromFile("D:/set09121_CW/res/RobotoMono-Regular.ttf")) {
        throw std::runtime_error("Failed to load font ");
    }
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(100, 100);

}

// Game Scene
void GameScene::update(double dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
        activeScene = menuScene;
    }

    for (auto& ent : _ents.list) {
        ent->update(dt);
    }

    text.setString("testing");
    text.setPosition(500, 400);


    Scene::update(dt);
}

void GameScene::render() {
    ls::render(Renderer::getWindow());

    for (auto& ent : _ents.list) {
        ent->render();
    }
    Renderer::queue(&text);
    Scene::render();
}


void GameScene::load() {
    ls::loadLevelFile("D:/set09121_CW/res/pacman.txt", 25.0f);

    // Create and set up player
    player = std::make_shared<Entity>(this);
    auto s = player->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(12.f);
    s->getShape().setFillColor(sf::Color::Yellow);
    s->getShape().setOrigin(sf::Vector2f(12.f, 12.f));
    player->addComponent<PlayerMovementComponent>();
    _ents.list.push_back(player);

    // Create and set up ghosts
    const sf::Color ghost_cols[]{ {208, 62, 25},    // red Blinky
                                 {219, 133, 28},   // orange Clyde
                                 {70, 191, 238},   // cyan Inky
                                 {234, 130, 229} }; // pink Pinky
    for (int i = 0; i < GHOSTS_COUNT; ++i) {
        auto ghost = std::make_shared<Entity>(this);
        auto sc = ghost->addComponent<ShapeComponent>();
        sc->setShape<sf::CircleShape>(12.f);
        sc->getShape().setFillColor(ghost_cols[i % 4]);
        sc->getShape().setOrigin(sf::Vector2f(12.f, 12.f));
        ghost->addComponent<EnemyAIComponent>(*player);
        ghosts.push_back(ghost);
        _ents.list.push_back(ghost);
    }

    respawn();
}

void GameScene::respawn() {
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    auto playerComponents = player->get_components<ActorMovementComponent>();
    if (!playerComponents.empty()) {
        playerComponents[0]->setSpeed(150.f);
    }

    auto ghost_spawns = ls::findTiles(ls::ENEMY);
    for (auto& g : ghosts) {
        g->setPosition(ls::getTilePosition(ghost_spawns[rand() % ghost_spawns.size()]));
        auto ghostComponents = g->get_components<ActorMovementComponent>();
        if (!ghostComponents.empty()) {
            ghostComponents[0]->setSpeed(100.0f);
        }

    }
}
// game.cpp
#include <SFML/Graphics.hpp>
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "system_renderer.h"
#include "scene.h"
#include "ecm.h"
#include "cmp_actor_movement.h"
#include "player_movement_component.h"
#include "LevelSystem.h"
#include "cmp_enemy_ai.h"
#include "player_projectile.h"

#include <vector>


#define Enemy_COUNT 4

std::vector<std::shared_ptr<Entity>>& Scene::getEnts() {
    return _ents.list;
}

std::shared_ptr<Scene> gameScene;
std::shared_ptr<Scene> menuScene;
std::shared_ptr<Scene> activeScene;

// Menu Scene
void MenuScene::update(double dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
        activeScene = gameScene;
    }
    Scene::update(dt);
    text.setString("Wizards Of The Abyss");
}

void MenuScene::render() {
    Renderer::queue(&backgroundSprite);
    Renderer::queue(&text);
    Scene::render();
}

void MenuScene::load() {
    // Load and set up the text element
    if (!font.loadFromFile("D:/set09121_CW/res/RobotoMono-Regular.ttf")) {
        throw std::runtime_error("Failed to load font ");
    }
    text.setFont(font);
    text.setCharacterSize(60);
    text.setFillColor(sf::Color::Black);
    text.setPosition(150, 300);

    // Load the background image
    if (!backgroundTexture.loadFromFile("D:/set09121_CW/res/img/book_pages.png")) {
        std::cerr << "Failed to load menu background image." << std::endl;
        // Handle the error, e.g., exit the program
    }
    else {
        // Set the texture to the sprite
        backgroundSprite.setTexture(backgroundTexture);
    }
}

// Game Scene
void GameScene::update(double dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
        activeScene = menuScene;
    }

    for (auto& ent : _ents.list) {
            ent->update(dt);
    }

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
    ls::loadLevelFile("D:/set09121_CW/res/levels/pacman.txt", 20.f);

    static sf::Texture projectileTexture;
    if (!projectileTexture.loadFromFile("D:/set09121_CW/res/img/Fireball-1.png")) {
        std::cerr << "Failed to load projectile texture" << std::endl;
    }

    sf::RenderWindow& window = Renderer::getWindow();

    // Create and set up player
    player = std::make_shared<Player>(this, window, projectileTexture);
    auto s = player->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(12.f);
    s->getShape().setFillColor(sf::Color::Yellow);
    s->getShape().setOrigin(sf::Vector2f(12.f, 12.f));
    player->addComponent<PlayerMovementComponent>();
    _ents.list.push_back(player);

    // Create and set up Enemys
    const sf::Color enemy_cols[]{ {208, 62, 25},    // red Blinky
                                 {219, 133, 28},   // orange Clyde
                                 {70, 191, 238},   // cyan Inky
                                 {234, 130, 229} }; // pink Pinky
    for (int i = 0; i < Enemy_COUNT; ++i) {
        auto enemy = std::make_shared<Entity>(this);
        auto sc = enemy->addComponent<ShapeComponent>();
        sc->setShape<sf::CircleShape>(12.f);
        sc->getShape().setFillColor(enemy_cols[i % 4]);
        sc->getShape().setOrigin(sf::Vector2f(12.f, 12.f));
        auto getPlayerPosition = [this]() -> sf::Vector2f { return this->player->getPosition(); };
        enemy->addComponent<EnemyAIComponent>(getPlayerPosition, 50.0f);
        enemys.push_back(enemy);
        _ents.list.push_back(enemy);
    }

    respawn();

}

void GameScene::respawn() {
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    auto playerComponents = player->get_components<ActorMovementComponent>();

    auto enemy_spawns = ls::findTiles(ls::ENEMY);
    for (auto& g : enemys) {
        g->setPosition(ls::getTilePosition(enemy_spawns[rand() % enemy_spawns.size()]));
        auto enemyComponents = g->get_components<EnemyAIComponent>();

    }
}

void Scene::addEntity(std::shared_ptr<Entity> entity) {
    if (!entity) {
        std::cerr << "Attempted to add a null entity to the scene" << std::endl;
        return;
    }

    _ents.list.push_back(entity);
}


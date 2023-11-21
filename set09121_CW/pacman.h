//pacman.h
#pragma once
#include "scene.h"

extern std::shared_ptr<Scene> gameScene;
extern std::shared_ptr<Scene> menuScene;
extern std::shared_ptr<Scene> activeScene;

class MenuScene : public Scene {
private:
	sf::Text text;
	sf::Font font;

public:
	MenuScene() = default;
	void update(double dt) override;
	void render() override;
	void load() override;
};

class GameScene : public Scene {
private:
    std::shared_ptr<Entity> player;
    std::vector<std::shared_ptr<Entity>> ghosts;
    sf::Text text;
    sf::Font font;

public:
    GameScene() = default;
    void update(double dt) override;
    void render() override;
    void load() override;
    void respawn();
};
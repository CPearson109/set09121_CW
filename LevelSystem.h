#pragma once
//LevelSystem.h
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include "maths.h"
#include <vector>
#include <queue>
#include <unordered_map>

class LevelSystem {
public:
    enum TILE { EMPTY, START, END, WALL, ENEMY, WAYPOINT };

    static size_t getWidth();
    static size_t getHeight();
    static void loadLevelFile(const std::string&, float tileSize = 35.f);
    static void Render(sf::RenderWindow& window);
    static sf::Color getColor(TILE t);
    static void setColor(TILE t, sf::Color c);
    static TILE getTile(sf::Vector2ul);
    static sf::Vector2f getTilePosition(sf::Vector2ul);
    static sf::Vector2f getStartTilePosition();  // Declare this method
    static sf::Vector2f getEnemyTilePosition();
    static float getTileSize();
    static std::vector<sf::Vector2f> LevelSystem::getAllStartTilePositions();
   


protected:
    static std::unique_ptr<TILE[]> _tiles; // Internal array of tiles
    static size_t _width; // how many tiles wide is level
    static size_t _height; // how many tiles high is level
    static float _tileSize; // Screenspace size of each tile, when rendered.
    static std::map<TILE, sf::Color> _colours; // color to render each tile type
    static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites; // array of SFML sprites of each tile
    static void buildSprites();

private:
    LevelSystem() = delete;
    ~LevelSystem() = delete;
};

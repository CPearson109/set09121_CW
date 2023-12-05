//LevelSystem.cpp
#include "LevelSystem.h"
#include "maths.h"
#include <fstream>
#include <iostream>
#include <sstream> // This is required for std::ostringstream
#include <stdexcept> 
#include <set>

using namespace std;
using namespace sf;

// Initialize the static members
std::unique_ptr<LevelSystem::TILE[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;

float LevelSystem::_tileSize(35.f);
vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

// Initialize the colors for different tiles
std::map<LevelSystem::TILE, sf::Color> LevelSystem::_colours{ {WALL, Color::White}, {END, Color::Red} };

size_t LevelSystem::getWidth() {
    return _width;
}

size_t LevelSystem::getHeight() {
    return _height;
}

// Get the color for a specific tile type
sf::Color LevelSystem::getColor(LevelSystem::TILE t) {
    auto it = _colours.find(t);
    if (it == _colours.end()) {
        _colours[t] = Color::Transparent;
    }
    return _colours[t];
}

void LevelSystem::setColor(LevelSystem::TILE t, sf::Color c) {
    _colours[t] = c;
}
void LevelSystem::Render(sf::RenderWindow& window) {
    for (const auto& sprite : _sprites) {
        window.draw(*sprite);
    }
}
void LevelSystem::loadLevelFile(const std::string& path, float tileSize) {
    _tileSize = tileSize;
    size_t w = 54;  // Set to the length of the longest line as observed in the file
    size_t h = 0;
    string line;
    

    ifstream f(path);
    if (!f.is_open()) {
        throw std::runtime_error("Couldn't open level file: " + path);
    }

    std::vector<TILE> temp_tiles;  // Define the vector to store tiles
    while (getline(f, line)) {
        h++;
        line.resize(w, ' '); // Resize line to width w, filling with spaces if shorter
        for (size_t col = 0; col < line.length(); col++) {
            char c = line[col];
            switch (c) {
            case 'w':
                temp_tiles.push_back(WALL);
                break;
            case 's':
                temp_tiles.push_back(START);
                break;
            case 'e':
                temp_tiles.push_back(END);
                break;
            case ' ':
                temp_tiles.push_back(EMPTY);  // Assuming space represents an EMPTY tile
                break;
            case '+':
                temp_tiles.push_back(WAYPOINT);
                break;
            case 'n':
                temp_tiles.push_back(ENEMY);
                break;
                // Add other cases as necessary
            default:if (col < line.length()) {  // Ensure column index is within line length
                cout << "Unknown tile at line " << h << ", column " << col + 1 << ": " << c << endl;
            }
                break;
            }
        }
    }
    f.close();

    std::cout << "Total lines (height) read: " << h<< std::endl;

   
    if (temp_tiles.size() != (w * h)) {
        std::cerr << "Error parsing level file. Expected tiles: " << (w * h)
            << ", but found: " << temp_tiles.size() << std::endl;
        throw std::runtime_error("Can't parse level file: " + path);
    }

    _tiles = std::make_unique<TILE[]>(w * h);
    _width = w;
    _height = h;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
    cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
    buildSprites();
}


void LevelSystem::buildSprites() {
    _sprites.clear();
    for (size_t y = 0; y < LevelSystem::getHeight(); ++y) {
        for (size_t x = 0; x < LevelSystem::getWidth(); ++x) {
            auto s = make_unique<RectangleShape>();
            s->setPosition(getTilePosition({ x, y }));
            s->setSize(Vector2f(_tileSize, _tileSize));

            // Check the tile type and set its fill color accordingly
            TILE tileType = getTile({ x, y });
            switch (tileType) {
            case WALL:
                s->setFillColor(Color::Transparent); // Make WALL tiles transparent
                break;
            case END:
                s->setFillColor(Color::Transparent); // Make END tiles transparent
                break;
            default:
                s->setFillColor(getColor(tileType)); // Set the fill color for other tiles
                break;
            }

            _sprites.push_back(move(s));
        }
    }
}


LevelSystem::TILE LevelSystem::getTile(sf::Vector2ul p) {
    if (p.x >= _width || p.y >= _height) {
        std::cout << "Requested tile at (" << p.x << ", " << p.y << ") in a level of size " << _width << "x" << _height << std::endl;
        throw std::out_of_range("Tile out of range: " + std::to_string(p.x) + "," + std::to_string(p.y));
    }
    return _tiles[(p.y * _width) + p.x];
}

sf::Vector2f LevelSystem::getTilePosition(Vector2ul p) {
    if (p.x >= _width || p.y >= _height) {
        std::ostringstream oss;
        oss << "Tile out of range: " << p.x << "," << p.y << ")";
        throw std::out_of_range(oss.str());
    }
    return sf::Vector2f(p.x * _tileSize, p.y * _tileSize);  // Correctly calculate the position
}

sf::Vector2f LevelSystem::getStartTilePosition() {
    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            if (_tiles[y * _width + x] == START) {  // Assuming START represents the 'S' tile
                return sf::Vector2f(static_cast<float>(x) * _tileSize, static_cast<float>(y) * _tileSize);
            }
        }
    }
    return sf::Vector2f(-1.f, -1.f);  // Return an invalid position if not found
}

sf::Vector2f LevelSystem::getEnemyTilePosition() {
    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            if (_tiles[y * _width + x] == ENEMY) {  // Assuming ENEMY represents the 'n' tile
                return sf::Vector2f(static_cast<float>(x) * _tileSize, static_cast<float>(y) * _tileSize);
            }
        }
    }
    return sf::Vector2f(-1.f, -1.f);  // Return an invalid position if not found
}

float LevelSystem::getTileSize() {
    return _tileSize;
}

std::vector<sf::Vector2f> LevelSystem::getAllStartTilePositions() {
    std::vector<sf::Vector2f> startPositions;
    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            if (_tiles[y * _width + x] == ENEMY) {
                startPositions.emplace_back(static_cast<float>(x) * _tileSize, static_cast<float>(y) * _tileSize);
            }
        }
    }
    return startPositions;
}

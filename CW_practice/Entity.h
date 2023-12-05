//Entity.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "maths.h" // Include this here if it's required for the Component or Entity
#include "EntityStats.h"
#include <memory>
#include <set>
#include <vector>
#include <typeindex>
#include <algorithm>

class Entity;

// Component class definition
class Component {
    friend class Entity;  // Making Entity a friend to access protected members

protected:
    Entity* _parent = nullptr;
    bool _fordeletion;  // Should this component be removed
    //explicit Component(Entity* const p);

public:
    Component() : _parent(nullptr), _fordeletion(false) {} // Default constructor
    virtual ~Component() = default; 
   

    void setParent(Entity* p) { _parent = p; }
    Entity* getParent() const { return _parent; }

    bool is_fordeletion() const;
    virtual void update(const float& dt) = 0;
    virtual void render() = 0;
};


class EntityManager;

class Entity : public sf::Sprite {
    // Grant EntityManager access to Entity's internals
    friend class EntityManager;
protected:
    sf::IntRect _sprite;
    sf::Texture _texture;
    std::vector<std::shared_ptr<Component>> _components;
    sf::Vector2f _position;
    float _rotation;
    bool _alive;       // Should be updated
    bool _visible;     // Should be rendered
    bool _fordeletion; // Should be deleted
    std::set<std::string> _tags;
    //Default constructor is hidden
    Entity();
public:
  
    static bool direction;
    static float speed;
    //Constructor that takes a sprite
    explicit Entity(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture); 
    //Pure virtual deconstructor -- makes this an abstract class and avoids undefined behaviour!
    virtual ~Entity() = 0;
    void addTag(const std::string& t);
    const std::set<std::string>& getTags() const;
    virtual void Update(const float& dt, sf::RenderWindow& window) = 0;
    //virtual void render();
    const sf::Vector2f& getPosition() const;
    void setPosition(const sf::Vector2f& _position);
    // Method to check if the entity is marked for deletion
    bool is_fordeletion() const { return _fordeletion; }

    // Add necessary getters and setters
    bool isAlive() const { return _alive; }
    void setAlive(bool value) { _alive = value; }

    bool isVisible() const { return _visible; }
    void setVisible(bool value) { _visible = value; }

    void setForDelete() {
        _fordeletion = true;
        _alive = false;
        _visible = false;
    }

    // Add rotation getters and setters if necessary
    float getRotation() const { return _rotation; }
    void setRotation(float value) { _rotation = value; }



    // Rename Update to update if necessary
    //virtual void Update(const float& dt, sf::RenderWindow& window) = 0;

    template <typename T, typename... Targs>
    std::shared_ptr<T> addComponent(Targs... params) {
        static_assert(std::is_base_of<Component, T>::value, "T must be a derived class of Component");
        auto comp = std::make_shared<T>(params...); // Create the component without 'this'
        comp->setParent(this);                     // Set the parent entity
        _components.push_back(comp);               // Add the component to the entity's list
        return comp;
    }

    template <typename T>
    const std::vector<std::shared_ptr<T>> get_components() const {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::vector<std::shared_ptr<T>> ret;
        for (const auto& c : _components) {
            if (typeid(*c) == typeid(T)) {
                ret.push_back(std::dynamic_pointer_cast<T>(c));
            }
        }
        return ret;
    }
};

// EntityManager struct definition
struct EntityManager {
    std::vector<std::shared_ptr<Entity>> list;

    void update(const float& dt, sf::RenderWindow& window);
    void render();
    std::vector<std::shared_ptr<Entity>> find(const std::string& tag) const;
    std::vector<std::shared_ptr<Entity>> find(const std::vector<std::string>& tags) const;
};



class Mage : public Entity {
public:
    Mage(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture, const EntityStats& stats); // Add EntityStats parameter

    void  Update(const float& dt, sf::RenderWindow& window) override;
    void FireBullet(bool mode, const sf::Vector2f& direction, const sf::Vector2f& pos);


    // Method to get the current stats of the Mage
    EntityStats getStats() const {
        return _entityStats;
    }

    // Method to set the stats of the Mage
    void setStats(const EntityStats& newStats) {
        _entityStats = newStats;
    }


private:
    Animation walkDownAnimation;
    Animation walkUpAnimation;
    Animation walkLeftAnimation;
    Animation walkRightAnimation;

    // Enumeration for directions
    enum class Direction { Up, Down, Left, Right };
    Direction currentDirection = Direction::Down; // Default direction

    //variable to store stats
    EntityStats _entityStats;

    // Speed of the mage
    float speed;
    float _shootTimer;

};


class Slime : public Entity {
public:
    Slime(sf::IntRect ir, sf::Vector2f pos, sf::Texture& texture, Entity& player, float speed, const EntityStats& stats);
    void Update(const float& dt, sf::RenderWindow& window) override;
    void FireBullet(bool mode, const sf::Vector2f& direction, const sf::Vector2f& pos);
    void onBulletHit();

private:
    Animation* currentAnimation; // Declare currentAnimation as a pointer to Animation
    Animation idleAnimation;
    Animation attackAnimation;
    Animation movingAnimation;
    float _speed;
    Entity& _player;
    float _shootTimer;
    int _health;

    //variable to store stats
    EntityStats _entityStats;

};




#include "Entity.h"



using namespace std;

void EntityManager::update(const float& dt, sf::RenderWindow& window) {
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i]->is_fordeletion()) {
            list.erase(list.begin() + i);
            --i;
            continue;
        }
        if (list[i]->_alive) {
            list[i]->Update(dt, window);
        }
    }
}


vector<shared_ptr<Entity>> EntityManager::find(const string& tag) const {
    vector<shared_ptr<Entity>> ret;
    for (auto& entity : list) {
        if (entity->_tags.find(tag) != entity->_tags.end()) {  // Direct access
            ret.push_back(entity);
        }
    }
    return ret;
}

vector<shared_ptr<Entity>> EntityManager::find(const vector<string>& tags) const {
    vector<shared_ptr<Entity>> ret;
    for (auto& entity : list) {
        const auto& tgs = entity->_tags;  // Direct access
        if (any_of(tags.begin(), tags.end(),
            [&tgs](const auto& t) { return tgs.find(t) != tgs.end(); })) {
            ret.push_back(entity);
        }
    }
    return ret;
}

module model;

import <vector>;
import entity;
import shape;
import <memory>;
import <algorithm>;
import <tuple>;

void Model::addEntity(std::unique_ptr<Entity*> entity) {
    if (entities.empty()) {
        entities.push_back(std::move(entity));
        return;
    }
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        if (it != entities.end() - 1) {
            if ((**it)->getShape().getZ() <= (**(it + 1))->getShape().getZ()) {
                entities.insert(it, std::move(entity));
                return;
            }
        } else {
            entities.push_back(std::move(entity));
            return;
        }
    }
}

void Model::removeEntity(Entity* entity){
    entities.erase(std::remove_if(entities.begin(), entities.end(),
    [entity](const std::unique_ptr<Entity*>& e) {
        return *e == entity;
    }), entities.end());
}

std::vector<std::unique_ptr<Entity*>>& Model::getEntities() {
    return entities;
}

int Model::getEntityCount() const {
    return entities.size();
}

int Model::getHeight() const {
    return height;
}

int Model::getWidth() const {
    return width;
}

int Model::getStartX() const {
    return start_x;
}

int Model::getStartY() const {
    return start_y;
}

bool Model::hasSolidBorders() const {
    return solid_borders;
}


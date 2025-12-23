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

Model::~Model() {
    for (auto& entity_ptr : entities) {
        delete *entity_ptr;
    }
    entities.clear();
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

void Model::updateEntities(int input) {
    std::vector<Entity*> toRemove;
    for (auto i = entities.begin(); i != entities.end(); ++i) {
        Entity* entity_i = **i;
        
        // For player entities, only update if it doesn't go out of bounds
        if (entity_i->isPlayer()) {
            auto [dx, dy] = entity_i->getMovedStepOne(input);
            if (!entity_i->getShape().goesOutOfBounds({dx, dy}, start_x, start_y, start_x + width, start_y + height)) {
                entity_i->update(input);
            } else if (solid_borders) {
                // Handle collision with border for player entities (only if solid borders are enabled)
                if (entity_i->has_collider()) {
                    entity_i->getColliderComponent()->setCollisionStatus(true);
                    entity_i->getColliderComponent()->doAction();
                }
            }
        } else {
            // For non-player entities, always update (allow them to leave the screen)
            entity_i->update(input);
            
            // If non-player entity goes out of bounds and has a collider, trigger doAction (only if solid borders)
            if (solid_borders && entity_i->has_collider() && 
                entity_i->getShape().isOutOfBounds(start_x, start_y, start_x + width, start_y + height)) {
                entity_i->getColliderComponent()->doAction();
            }
        }
        // For non-solid borders, allow movement out of bounds (handled later in offscreen tracking)
        entity_i->setHasMovedFlag(true);
        
        // For players, constrain to screen boundaries (with margin for border)
        if (entity_i->isPlayer()) {
            int x = entity_i->getShape().getX();
            int y = entity_i->getShape().getY();
            bool adjusted = false;
            
            // Clamp player position to screen bounds with 1-unit margin for border
            // Left boundary (account for border on left)
            if (x < start_x + 1) {
                x = start_x + 1;
                adjusted = true;
                while (entity_i->getShape().outOfBoundsX(start_x, start_x + width, x)) {
                    x += 1;
                }
            } 
            // Right boundary (account for border on right)
            else if (x > start_x + width - 2) {
                x = start_x + width - 2;
                adjusted = true;
                while (entity_i->getShape().outOfBoundsX(start_x, start_x + width, x)) {
                    x -= 1;
                }
            }
            
            // Top boundary (account for border on top)
            if (y < start_y + 1) {
                y = start_y + 1;
                adjusted = true;
                while (entity_i->getShape().outOfBoundsY(start_y, start_y + height, y)) {
                    y += 1;
                }
            } 
            // Bottom boundary (account for border on bottom)
            else if (y > start_y + height - 2) {
                y = start_y + height - 2;
                adjusted = true;
                while (entity_i->getShape().outOfBoundsY(start_y, start_y + height, y)) {
                    y -= 1;
                }
            }
            
            // Set position only if adjusted
            if (adjusted) {
                entity_i->getShape().setPosition(x, y);
            }
        }
        
        // Check collision with other entities
        for (auto j = entities.begin(); j != entities.end(); ++j) {
            if (i == j) continue; // Skip self
            
            Entity* entity_j = **j;
            
            // Only check collision if entities are on the same Z-level and both have colliders
            if (entity_i->getShape().getZ() == entity_j->getShape().getZ() &&
                entity_i->has_collider() && entity_j->has_collider()) {
                if (entity_i->getShape().overlaps(entity_j->getShape(),
                        {entity_i->getShape().getX(), entity_i->getShape().getY()},
                        {entity_j->getShape().getX(), entity_j->getShape().getY()})) {
                    // Collision detected - call hasCollided on both entities
                    entity_i->hasCollided();
                    entity_j->hasCollided();
                    break;
                }
            }
        }
        
        // Handle solid border collisions
        if (solid_borders && entity_i->has_collider()) {
            if (entity_i->getShape().isOutOfBounds(start_x, start_y, start_x + width, start_y + height)) {
                entity_i->getColliderComponent()->setCollisionStatus(true);
                entity_i->getColliderComponent()->doAction();
            }
        }
        
        // Handle offscreen tracking (only for non-solid borders)
        if (!solid_borders) {
            if (entity_i->getShape().isOutOfBounds(start_x, start_y, start_x + width, start_y + height)) {
                if (entity_i->getOffscreen()) {
                    entity_i->incrementOffscreenCounter();
                } else {
                    entity_i->setOffscreen(true);
                    entity_i->resetOffscreenCounter();
                }
                if (entity_i->getOffscreenCounter() > entity_i->getMaxOffscreenAllowed()) {
                    toRemove.push_back(entity_i);
                }
            } else if (entity_i->getOffscreen() && !entity_i->getShape().isOutOfBounds(start_x, start_y, start_x + width, start_y + height)) {
                entity_i->setOffscreen(false);
                entity_i->resetOffscreenCounter();
            }
        }
    }
    for (auto& entity_ptr : toRemove) {
        removeEntity(entity_ptr);
    }
}

void Model::resetEntitiesAfterTick() {
    for (auto& entity_ptr : entities) {
        (*entity_ptr)->setHasMovedFlag(false);
        (*entity_ptr)->resetStepsMoved();
        (*entity_ptr)->resetColliderStatus();
    }
}

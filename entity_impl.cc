module entity;

import <utility>; // done
import <vector>; // done
import <optional>; // done
import move_strategy; // done
import ncurses_win; // done
import shape; // done
import cyclic_transformations;
import abstract_collider;
import linear_move_strategy;
import player_movement_strat;

Entity::~Entity() {
    delete moveStrategy; // it owns the strategy so it should delete it
    if (colliderComponent.has_value()) {
        delete *colliderComponent;
    }
}

std::pair<int, int> Entity::getPosition() const {
    return {shape.getX(), shape.getY()};
}

void Entity::setPosition(int x, int y) {
    shape.setPosition(x, y);
}

void Entity::update(int input) {
    if (!has_moved) {
        moveStrategy->move(input);
        steps_moved_x = 0;
        steps_moved_y = 0;
    } else {
        shape.setPosition(shape.getX() + steps_moved_x, shape.getY() + steps_moved_y);
    }
}

void Entity::addMoveStrategy(MoveStrategy* strategy) {
    if (moveStrategy) {
        strategy->setNextStrategy(moveStrategy);
    }
    moveStrategy = strategy; // this basically takes ownership of the new strategy
}

std::pair<int, int> Entity::getNextPosition(int input) {
    // assuming moveStrategy is not null(reasonable as entity should have a movement strategy)
    if (has_moved) {
        return {shape.getX() + steps_moved_x, shape.getY() + steps_moved_y};
    } else {
        auto [nxt_x, nxt_y] = moveStrategy->getnextPosition(input);
        steps_moved_x = nxt_x - shape.getX();
        steps_moved_y = nxt_y - shape.getY();
        return {nxt_x, nxt_y};
    }
}

void Entity::render(Window& win, Shape& shp) const {
    for (const auto& [offset, ch] : shp.getCharMap()) {
        int draw_x = shp.getX() + offset.first;
        int draw_y = shp.getY() + offset.second;
        if (draw_x >= 0 && draw_y >= 0 && draw_x < win.getMaxx() && draw_y < win.getMaxy()) {
            win.mvWaddch(draw_y, draw_x, ch);
        }
    }
}

Shape& Entity::getShape() {
    return shape;
}

void Entity::removeCyclicTransformationStrategies() {
    MoveStrategy* current = moveStrategy;
    MoveStrategy* prev = nullptr;

    while (current) {
        // Check if current strategy is a cyclic transformation
        if (dynamic_cast<CyclicTransformation*>(current)) {
            // Remove current strategy from the chain
            if (prev) {
                prev->setNextStrategy(current->getNextStrategy());
            } else {
                moveStrategy = current->getNextStrategy();
            }
            MoveStrategy* toDelete = current;
            current = current->getNextStrategy();
            toDelete->setNextStrategy(nullptr); // avoiding cascading deletions
            delete toDelete; // free memory
        } else {
            prev = current;
            current = current->getNextStrategy();
        }
    }
    shape.setcharMap(originalShape.getCharMap());
}

void Entity::resetMoveStrategies() {
    delete moveStrategy;
    moveStrategy = new LinearMoveStrategy(shape, nullptr, std::make_pair<int, int>(0, 0)); // reset to default linear strategy with no movement
}

std::pair<int, int> Entity::getOneStep(int input) {
    if (moveStrategy) {
        auto [next_x, next_y] = moveStrategy->getnextPosition(input);
        // get relative movement
        next_x -= shape.getX();
        next_y -= shape.getY();
        if (next_x > 0) {
            next_x = 1;
        } else if (next_x < 0) {
            next_x = -1;
        } else {
            next_x = 0;
        }
        if (next_y > 0) {
            next_y = 1;
        } else if (next_y < 0) {
            next_y = -1;
        } else {
            next_y = 0;
        }
        return {next_x, next_y};
    }
    return {0, 0}; // no movement strategy, stay in place
}

void Entity::takeOneStep(int input) {
    if (moveStrategy) {
        auto [next_x, next_y] = Entity::getOneStep(input);
        steps_moved_x -= next_x;
        steps_moved_y -= next_y;
        shape.setPosition(shape.getX() + next_x, shape.getY() + next_y);
    }
}

void Entity::addColliderComponent(AbstractCollider* collider) {
    if (!colliderComponent.has_value()) {
        colliderComponent = collider;
    } else {
        delete *colliderComponent;
        *colliderComponent = collider;
    }
}

AbstractCollider* Entity::getColliderComponent() {
    if (colliderComponent.has_value()) {
        return *colliderComponent;
    }
}

bool Entity::has_collider() {
    return colliderComponent.has_value();
}

void Entity::resetColliderStatus() {
    if (colliderComponent.has_value()) {
        (*colliderComponent)->setCollisionStatus(false);
    }
}

bool Entity::isPlayer() {
    return is_player;
}

void Entity::removeNonSpecialStrategies() {
    MoveStrategy* current = moveStrategy;
    MoveStrategy* prev = nullptr;

    while (current) {
        // Check if current strategy is neither player control nor cyclic transformation
        if (!dynamic_cast<CyclicTransformation*>(current) &&
            !dynamic_cast<PlayerMovementStrategy*>(current)) {
            // Remove current strategy from the chain
            if (prev) {
                prev->setNextStrategy(current->getNextStrategy());
            } else {
                moveStrategy = current->getNextStrategy();
            }
            MoveStrategy* toDelete = current;
            current = current->getNextStrategy();
            toDelete->setNextStrategy(nullptr); // avoid cascading deletions
            delete toDelete; // free memory
        } else {
            prev = current;
            current = current->getNextStrategy();
        }
    }
}

void Entity::setOffscreen(bool status) {
    is_offscreen = status;
}

bool Entity::getOffscreen() const {
    return is_offscreen;
}

void Entity::incrementOffscreenCounter() {
    offscreen_counter++;
}

int Entity::getOffscreenCounter() const {
    return offscreen_counter;
}

void Entity::resetOffscreenCounter() {
    offscreen_counter = 0;
}

bool Entity::getHasMovedFlag() {
    return has_moved;
}

void Entity::setHasMovedFlag(bool status) {
    has_moved = status;
}

void Entity::incrementStepsMoved(int dx, int dy) {
    steps_moved_x += dx;
    steps_moved_y += dy;
}

std::pair<int, int> Entity::getStepsMoved() const {
    return {steps_moved_x, steps_moved_y};
}

void Entity::resetStepsMoved() {
    steps_moved_x = 0;
    steps_moved_y = 0;
}

int Entity::getMaxOffscreenAllowed() const {
    return max_offscreen_counter_allowed;
}

std::pair<int, int> Entity::getMovedStepOne(int input) {
    int dx = 0;
    int dy = 0;
    if (has_moved) {
        if (steps_moved_x > 0) {
            dx = 1;
        } else if (steps_moved_x < 0) {
            dx = -1;
        }
        if (steps_moved_y > 0) {
            dy = 1;
        } else if (steps_moved_y < 0) {
            dy = -1;
        }
    } else {
        auto [next_x, next_y] = moveStrategy->getnextPosition(input);
        dx = next_x - shape.getX();
        dy = next_y - shape.getY();
        if (dx > 0) {
            dx = 1;
        } else if (dx < 0) {
            dx = -1;
        } else {
            dx = 0;
        }
        if (dy > 0) {
            dy = 1;
        } else if (dy < 0) {
            dy = -1;
        } else {
            dy = 0;
        }
    }
    return {dx, dy};
}

void Entity::takeMovedStepOne(int input) {
    auto [dx, dy] = getMovedStepOne(input);
    has_moved = true;
    steps_moved_x -= dx;
    steps_moved_y -= dy;
    shape.setPosition(shape.getX() + dx, shape.getY() + dy);
}

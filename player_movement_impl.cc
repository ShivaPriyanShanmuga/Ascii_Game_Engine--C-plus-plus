module player_movement_strat;


import move_strategy;
import <utility>;
import shape;
import <unordered_map>;


PlayerMovementStrategy::~PlayerMovementStrategy() {
    delete nextStrategy;
}

void PlayerMovementStrategy::move(int input)  {
    auto it = movement_map.find(input);
    if (it != movement_map.end()) {
        auto [dx, dy] = it->second;
        int current_x = shape.getX();
        int current_y = shape.getY();
        shape.setPosition(current_x + dx, current_y + dy);
        if (nextStrategy) {
            nextStrategy->move(input); // propagate movement to next strategy if exists
        }
    } else {
        // No movement for unrecognized input, but still apply velocity(there cud be more than one strat beyond this one)
        if (nextStrategy) {
            nextStrategy->move(input); // there can be no velocity directly applied to player
        }
    }
}

// note: there can only be one player movement strategy attached to an entity
std::pair<int, int> PlayerMovementStrategy::getchange(int input) {
    auto it = movement_map.find(input);
    if (it != movement_map.end()) {
        return it->second;
    } else {
        return {0, 0};
    }
}

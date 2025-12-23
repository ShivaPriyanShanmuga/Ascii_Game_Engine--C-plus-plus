module linear_move_strategy;

import move_strategy;
import shape;
import <optional>;
import <utility>;

void LinearMoveStrategy::move(int input) {
    int x = shape.getX();
    int y = shape.getY();
    // assuming velocity is always set for LinearMoveStrategy
    int vx = velocity->first; 
    int vy = velocity->second;
    shape.setPosition(x + vx, y + vy);
    if (nextStrategy) {
        nextStrategy->move(input);
    }
}

LinearMoveStrategy::~LinearMoveStrategy() {delete nextStrategy;}

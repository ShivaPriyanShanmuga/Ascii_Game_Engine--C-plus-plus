module gravity_movement;

import move_strategy;
import <optional>;
import <utility>;
import shape;

GravityMoveStrategy::~GravityMoveStrategy() {delete nextStrategy;}

void GravityMoveStrategy::move(int input) {
    tick_count++;
    if (tick_count > gravity_interval) {
        tick_count = 0;
        int x = shape.getX();
        int y = shape.getY();
        int one_step_y = 0;
        int one_step_x = 0;
        if (velocity->first != 0) {
            one_step_x = (velocity->first > 0) ? 1 : -1;
        }
        if (velocity->second != 0) {
            one_step_y = (velocity->second > 0) ? 1 : -1;
        }
        shape.setPosition(x + velocity->first, y + velocity->second);
        if (nextStrategy) {
            nextStrategy->move(input);
        }
    }
}

export module gravity_movement;

import move_strategy;
import <optional>;
import <utility>;
import shape;

export class GravityMoveStrategy : public MoveStrategy {
    int gravity_interval;
    int tick_count = 0;
public:
    GravityMoveStrategy(Shape& shp, MoveStrategy* nextstrat = nullptr, std::optional<std::pair<int, int>> vel = std::pair<int, int>{0, 1}, int gravityInterval = 0) // default gravity downwards
        : MoveStrategy(shp, nextstrat, vel), gravity_interval(gravityInterval) {}
    virtual ~GravityMoveStrategy();
    virtual void move(int input) override;
};

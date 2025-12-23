export module linear_move_strategy;

import move_strategy;
import <optional>;
import <utility>;
import shape;

export class LinearMoveStrategy : public MoveStrategy {
public:
    LinearMoveStrategy(Shape& shp, MoveStrategy* nextstrat = nullptr, std::optional<std::pair<int, int>> vel = std::nullopt)
        : MoveStrategy(shp, nextstrat, vel) {}
    virtual ~LinearMoveStrategy();
    virtual void move(int input) override;
};

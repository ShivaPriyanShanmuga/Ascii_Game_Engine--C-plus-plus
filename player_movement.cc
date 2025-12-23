export module player_movement_strat;

import move_strategy;
import <utility>;
import shape;
import <unordered_map>;
import <optional>;

#include <ncurses.h>

export class PlayerMovementStrategy : public MoveStrategy {
    std::unordered_map<int, std::pair<int, int>> movement_map;
public:
    PlayerMovementStrategy(Shape& shp, MoveStrategy* nextstrat = nullptr, std::optional<std::pair<int, int>> vel = std::nullopt, std::unordered_map<int, std::pair<int, int>> input_map = {
        {KEY_UP, {0, -1}},
        {KEY_DOWN, {0, 1}},
        {KEY_LEFT, {-1, 0}},
        {KEY_RIGHT, {1, 0}}
    })
        : MoveStrategy(shp, nextstrat, vel), movement_map(input_map) {}

    ~PlayerMovementStrategy();

    void move(int input) override;

    std::pair<int, int> getchange(int input) override;
};

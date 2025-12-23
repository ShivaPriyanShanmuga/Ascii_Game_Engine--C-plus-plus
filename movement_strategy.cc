export module move_strategy;

import shape;
import <vector>;
import <utility>;
import <optional>;

export class MoveStrategy { 
protected:
    Shape& shape;
    MoveStrategy* nextStrategy;
    std::optional<std::pair<int, int>> velocity; // vx, vy

    // some recursive functions to get cumulative velocity
    inline int getxvelocity() const {
        if (nextStrategy) {
            return velocity.has_value() ? velocity->first + nextStrategy->getxvelocity() : nextStrategy->getxvelocity();
        } else {
            return velocity.has_value() ? velocity->first : 0;
        }
    }

    inline int getyvelocity() const {
        if (nextStrategy) {
            return velocity.has_value() ? velocity->second + nextStrategy->getyvelocity() : nextStrategy->getyvelocity();
        } else {
            return velocity.has_value() ? velocity->second : 0;
        }
    }

public:
    MoveStrategy(Shape& shp, MoveStrategy* nextstrat = nullptr, std::optional<std::pair<int, int>> vel = std::nullopt) : shape(shp), nextStrategy(nextstrat), velocity(vel) {}
    virtual ~MoveStrategy() = default;

    virtual void move(int input) = 0;

    std::pair<int, int> getnextPosition(int input) {
        int x = shape.getX();
        int y = shape.getY();
        return {x + getxvelocity() + this->getchange(input).first, y + getyvelocity() + this->getchange(input).second};
    }

    virtual std::pair<int, int> getchange(int input) {
        if (nextStrategy) {
            return nextStrategy->getchange(input);
        } else {
            return {0, 0};
        }
    };

    void setNextStrategy(MoveStrategy* nextstrat) {
        nextStrategy = nextstrat;
    }

    MoveStrategy* getNextStrategy() const {
        return nextStrategy;
    }
};

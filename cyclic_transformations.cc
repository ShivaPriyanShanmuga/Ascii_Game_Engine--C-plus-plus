export module cyclic_transformations;

import move_strategy;
import <vector>;
import <utility>;
import shape;

export class CyclicTransformation : public MoveStrategy {
    const std::vector<Shape>& transformations;
    size_t currentIndex;
    int cycleCounter;
    int cycleFrequency;
public:
    CyclicTransformation(Shape& shp, const std::vector<Shape>& transfs, int frequency = 1, MoveStrategy* nextstrat = nullptr): MoveStrategy(shp, nextstrat), transformations(transfs), currentIndex(0), cycleCounter(0), cycleFrequency(frequency) {}
    ~CyclicTransformation();

    void move(int input) override;

};

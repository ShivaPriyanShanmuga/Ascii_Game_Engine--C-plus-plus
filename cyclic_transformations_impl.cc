module cyclic_transformations;

import move_strategy;
import <vector>;
import <utility>;
import shape;

CyclicTransformation::~CyclicTransformation() {
    delete nextStrategy;
}

void CyclicTransformation::move(int input) {
    // Apply the current transformation to the entity's shape
    shape.setcharMap(transformations[currentIndex].getCharMap());
    
    // Increment the counter
    cycleCounter++;
    
    // Only update the index when the counter reaches the cycle frequency
    if (cycleCounter >= cycleFrequency) {
        cycleCounter = 0;
        // Update the index to the next transformation in a cyclic manner
        currentIndex = (currentIndex + 1) % transformations.size();
    }
}

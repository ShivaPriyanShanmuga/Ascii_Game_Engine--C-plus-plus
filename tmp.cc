void Model::updateEntities(int input) {
    for (auto i = entities.begin(); i != entities.end(); ++i) {
        auto [next_x, next_y] = (**i)->getNextPosition(input);
        bool collision = false;
        bool border_collision_i = false;
        for (auto j = i + 1; j != entities.end(); ++j) {
            auto [other_next_x, other_next_y] = (**j)->getNextPosition(input);
            bool border_collision_j = false;
            if ((**i)->has_collider() && (**j)->has_collider()) {
                if ((**i)->getShape().overlaps((**j)->getShape(), {next_x, next_y}, {other_next_x, other_next_y}) || 
                    (((**i)->getShape().getX() + next_x < start_x) ||
                    ((**i)->getShape().getX() + next_x >= start_x + width) ||
                    ((**i)->getShape().getY() + next_y < start_y) ||
                    ((**i)->getShape().getY() + next_y >= start_y + height) ||
                    ((**j)->getShape().getX() + other_next_x < start_x) ||
                    ((**j)->getShape().getX() + other_next_x >= start_x + width) ||
                    ((**j)->getShape().getY() + other_next_y < start_y) ||
                    ((**j)->getShape().getY() + other_next_y >= start_y + height))) {
                    auto [one_step_x, one_step_y] = (**i)->getOneStep(input);
                    auto [other_one_step_x, other_one_step_y] = (**j)->getOneStep(input);
                    while (!(**i)->getShape().overlaps((**j)->getShape(), 
                                    {(**i)->getShape().getX() + one_step_x, (**i)->getShape().getY() + one_step_y},
                                    {(**j)->getShape().getX() + other_one_step_x, (**j)->getShape().getY() + other_one_step_y}) && !(border_collision_i && border_collision_j)) {
                        if (solid_borders || (**i)->isPlayer() || (**j)->isPlayer()) {
                            if ( ((**i)->getShape().getX() + one_step_x < start_x) ||
                                 ((**i)->getShape().getX() + one_step_x >= start_x + width) ||
                                 ((**i)->getShape().getY() + one_step_y < start_y) ||
                                 ((**i)->getShape().getY() + one_step_y >= start_y + height) ) {
                                border_collision_i = true;
                            } 
                            if ( ((**j)->getShape().getX() + other_one_step_x < start_x) ||
                                 ((**j)->getShape().getX() + other_one_step_x >= start_x + width) ||
                                 ((**j)->getShape().getY() + other_one_step_y < start_y) ||
                                 ((**j)->getShape().getY() + other_one_step_y >= start_y + height) ) {
                                border_collision_j = true;
                            }
                        }
                        if (!border_collision_i) {
                            (**i)->takeOneStep(input);
                            [one_step_x, one_step_y] = (**i)->getOneStep(input);
                        }
                        if (!border_collision_j) {
                            (**j)->takeOneStep(input);
                            [other_one_step_x, other_one_step_y] = (**j)->getOneStep(input);
                        }
                    }
                    (**i)->getColliderComponent()->setCollisionStatus(true);
                    (**j)->getColliderComponent()->setCollisionStatus(true);
                    (**i)->getColliderComponent()->doAction();
                    (**j)->getColliderComponent()->doAction();
                    collision = true;
                }
            } else {
                if (solid_borders) {
                    if ( ((**i)->getShape().getX() + next_x < start_x) ||
                         ((**i)->getShape().getX() + next_x >= start_x + width) ||
                         ((**i)->getShape().getY() + next_y < start_y) ||
                         ((**i)->getShape().getY() + next_y >= start_y + height) ) {
                        border_collision_i = true;
                    } 
                    if ( ((**j)->getShape().getX() + other_next_x < start_x) ||
                         ((**j)->getShape().getX() + other_next_x >= start_x + width) ||
                         ((**j)->getShape().getY() + other_next_y < start_y) ||
                         ((**j)->getShape().getY() + other_next_y >= start_y + height) ) {
                        border_collision_j = true;
                    }
                    auto [one_step_x, one_step_y] = (**i)->getOneStep(input);
                    
                    while (!(((**i)->getShape().getX() + one_step_x < start_x) ||
                             ((**i)->getShape().getX() + one_step_x >= start_x + width) ||
                             ((**i)->getShape().getY() + one_step_y < start_y) ||
                             ((**i)->getShape().getY() + one_step_y >= start_y + height)) && border_collision_i) {
                        (**i)->takeOneStep(input);
                        [one_step_x, one_step_y] = (**i)->getOneStep(input);
                    }
                    auto [other_one_step_x, other_one_step_y] = (**j)->getOneStep(input);
                    while (!(((**j)->getShape().getX() + other_one_step_x < start_x) ||
                             ((**j)->getShape().getX() + other_one_step_x >= start_x + width) ||
                             ((**j)->getShape().getY() + other_one_step_y < start_y) ||
                             ((**j)->getShape().getY() + other_one_step_y >= start_y + height)) && border_collision_j) {
                        (**j)->takeOneStep(input);
                        [other_one_step_x, other_one_step_y] = (**j)->getOneStep(input);
                    }

                    if ((**i)->has_collider()) {
                        (**i)->getColliderComponent()->setCollisionStatus(true);
                        (**i)->getColliderComponent()->doAction();
                    }
                    if ((**j)->has_collider()) {
                        (**j)->getColliderComponent()->setCollisionStatus(true);
                        (**j)->getColliderComponent()->doAction();
                    }
                }
            }
        }
        if (!collision && !border_collision_i) {
            (**i)->update(input);
        }
    }
}

// this is the 2nd try
void Model::updateEntities(int input) {
    std::vector<Entity*> toRemove;
    for (auto i = entities.begin(); i != entities.end(); ++i) {
        auto [next_x, next_y] = (**i)->getNextPosition(input);
        bool collision = false;
        for (auto j = i + 1; j != entities.end(); ++j) {
            auto [other_next_x, other_next_y] = (**j)->getNextPosition(input);
            if ((**i)->has_collider() && (**j)->has_collider()) {
                if ((**i)->getShape().overlaps((**j)->getShape(),
                                                {(**i)->getShape().getX() + next_x, (**i)->getShape().getY() + next_y},
                                                {(**j)->getShape().getX() + other_next_x, (**j)->getShape().getY() + other_next_y}) ||
                                            (**i)->getShape().goesOutOfBounds(
                                                {next_x, next_y},
                                                start_x, start_y, start_x + width, start_y + height) ||
                                            (**j)->getShape().goesOutOfBounds(
                                                {other_next_x, other_next_y},
                                                start_x, start_y, start_x + width, start_y + height)) {
                    
                    auto [one_step_x, one_step_y] = (**i)->getMovedStepOne(input);
                    auto [other_one_step_x, other_one_step_y] = (**j)->getMovedStepOne(input);
                    bool bool_i_has_collided = false;
                    bool bool_j_has_collided = false;
                    if ((**i)->isPlayer() && (**j)->isPlayer()) {
                        while (!(**i)->getShape().overlaps((**j)->getShape(),
                                                {(**i)->getShape().getX() + one_step_x, (**i)->getShape().getY() + one_step_y},
                                                {(**j)->getShape().getX() + other_one_step_x, (**j)->getShape().getY() + other_one_step_y}) &&
                                !((**i)->getShape().goesOutOfBounds(
                                    {one_step_x, one_step_y},
                                    start_x, start_y, start_x + width, start_y + height))
                                &&
                                !((**j)->getShape().goesOutOfBounds(
                                    {other_one_step_x, other_one_step_y},
                                    start_x, start_y, start_x + width, start_y + height))) {
                            (**i)->takeMovedStepOne(input);
                            (**j)->takeMovedStepOne(input);
                            std::tie(one_step_x, one_step_y) = (**i)->getMovedStepOne(input);
                            std::tie(other_one_step_x, other_one_step_y) = (**j)->getMovedStepOne(input);
                        }
                        if ((**i)->getShape().goesOutOfBounds(
                                {one_step_x, one_step_y},
                                start_x, start_y, start_x + width, start_y + height)) {
                            bool_i_has_collided = true;
                        }
                        if ((**j)->getShape().goesOutOfBounds(
                                {other_one_step_x, other_one_step_y},
                                start_x, start_y, start_x + width, start_y + height)) {
                            bool_j_has_collided = true;
                        }
                        while (!(**i)->getShape().overlaps((**j)->getShape(),
                                                {(**i)->getShape().getX() + one_step_x, (**i)->getShape().getY() + one_step_y},
                                                {(**j)->getShape().getX() + other_one_step_x, (**j)->getShape().getY() + other_one_step_y})) {
                            if (!bool_i_has_collided) {
                                (**i)->takeMovedStepOne(input);
                                std::tie(one_step_x, one_step_y) = (**i)->getMovedStepOne(input);
                            }
                            if (!bool_j_has_collided) {
                                (**j)->takeMovedStepOne(input);
                                std::tie(other_one_step_x, other_one_step_y) = (**j)->getMovedStepOne(input);
                            }
                        }
                    } else if((**i)->isPlayer() && !(**j)->isPlayer()) {
                        while (!(**i)->getShape().overlaps((**j)->getShape(),
                                                {(**i)->getShape().getX() + one_step_x, (**i)->getShape().getY() + one_step_y},
                                                {(**j)->getShape().getX() + other_one_step_x, (**j)->getShape().getY() + other_one_step_y}) &&
                                !(**i)->getShape().goesOutOfBounds(
                                    {one_step_x, one_step_y},
                                    start_x, start_y, start_x + width, start_y + height)) {
                            (**i)->takeMovedStepOne(input);
                            std::tie(one_step_x, one_step_y) = (**i)->getMovedStepOne(input);
                            (**j)->takeMovedStepOne(input);
                            std::tie(other_one_step_x, other_one_step_y) = (**j)->getMovedStepOne(input);
                        }
                        while (!(**j)->getShape().overlaps((**i)->getShape(),
                                                {(**j)->getShape().getX() + other_one_step_x, (**j)->getShape().getY() + other_one_step_y},
                                                {(**i)->getShape().getX() + one_step_x, (**i)->getShape().getY() + one_step_y})) {
                            (**j)->takeMovedStepOne(input);
                            std::tie(other_one_step_x, other_one_step_y) = (**j)->getMovedStepOne(input);
                        }
                    } else if(!(**i)->isPlayer() && (**j)->isPlayer()) {
                        while (!(**i)->getShape().overlaps((**j)->getShape(),
                                                {(**i)->getShape().getX() + one_step_x, (**i)->getShape().getY() + one_step_y},
                                                {(**j)->getShape().getX() + other_one_step_x, (**j)->getShape().getY() + other_one_step_y}) &&
                                !(**j)->getShape().goesOutOfBounds(
                                    {other_one_step_x, other_one_step_y},
                                    start_x, start_y, start_x + width, start_y + height)) {
                            (**i)->takeOneStep(input);
                            std::tie(one_step_x, one_step_y) = (**i)->getOneStep(input);
                            (**j)->takeOneStep(input);
                            std::tie(other_one_step_x, other_one_step_y) = (**j)->getOneStep(input);
                        }
                        while (!(**i)->getShape().overlaps((**j)->getShape(),
                                                {(**i)->getShape().getX() + one_step_x, (**i)->getShape().getY() + one_step_y},
                                                {(**j)->getShape().getX() + other_one_step_x, (**j)->getShape().getY() + other_one_step_y})) {
                            (**i)->takeOneStep(input);
                            std::tie(one_step_x, one_step_y) = (**i)->getOneStep(input);
                        }
                    } else {
                        while (!(**i)->getShape().overlaps((**j)->getShape(),
                                                {(**i)->getShape().getX() + one_step_x, (**i)->getShape().getY() + one_step_y},
                                                {(**j)->getShape().getX() + other_one_step_x, (**j)->getShape().getY() + other_one_step_y})) {
                            (**i)->takeOneStep(input);
                            std::tie(one_step_x, one_step_y) = (**i)->getOneStep(input);
                            (**j)->takeOneStep(input);
                            std::tie(other_one_step_x, other_one_step_y) = (**j)->getOneStep(input);
                        }
                    }
                    
                    (**i)->setHasMovedFlag(true);
                    (**j)->setHasMovedFlag(true);
                    (**i)->getColliderComponent()->setCollisionStatus(true);
                    (**i)->getColliderComponent()->doAction();
                    (**j)->getColliderComponent()->setCollisionStatus(true);
                    (**j)->getColliderComponent()->doAction();
                    collision = true;
                } else {
                    (**j)->update(input);
                    (**j)->setHasMovedFlag(true);
                    (**i)->update(input);
                    (**i)->setHasMovedFlag(true);
                }
            } else {
                if ((**i)->isPlayer()) {
                    if ((**i)->getShape().goesOutOfBounds(
                            {next_x, next_y},
                            start_x, start_y, start_x + width, start_y + height)) {
                        auto [one_step_x, one_step_y] = (**i)->getOneStep(input);
                        while (!((**i)->getShape().goesOutOfBounds(
                                    {one_step_x, one_step_y},
                                    start_x, start_y, start_x + width, start_y + height)) ) {
                            (**i)->takeOneStep(input);
                            std::tie(one_step_x, one_step_y) = (**i)->getOneStep(input);
                        }
                        (**i)->setHasMovedFlag(true);
                        if (solid_borders) {
                            if ((**i)->has_collider()) {
                                (**i)->getColliderComponent()->setCollisionStatus(true);
                                (**i)->getColliderComponent()->doAction();
                            }
                            collision = true;
                        }
                    } else {
                        (**i)->update(input);
                        (**i)->setHasMovedFlag(true);
                    }
                } else {
                    // case where entity i is not a player
                    (**i)->update(input);
                    (**i)->setHasMovedFlag(true);
                    if (solid_borders) {
                        if ((**i)->getShape().isOutOfBounds(start_x, start_y, start_x + width, start_y + height)) {
                            if ((**i)->has_collider()) {
                                (**i)->getColliderComponent()->setCollisionStatus(true);
                                (**i)->getColliderComponent()->doAction();
                            }
                            (**i)->setOffscreen(true);
                            collision = true;
                        }
                    } else {
                        if ((**i)->getShape().isOutOfBounds(start_x, start_y, start_x + width, start_y + height)) {
                            (**i)->setOffscreen(true);
                        } else {
                            (**i)->setOffscreen(false);
                        }
                    }
                }
                if ((**j)->isPlayer()) {
                    if ((**j)->getShape().goesOutOfBounds(
                            {other_next_x, other_next_y},
                            start_x, start_y, start_x + width, start_y + height)) {
                        auto [other_one_step_x, other_one_step_y] = (**j)->getMovedStepOne(input);
                        while (!((**j)->getShape().goesOutOfBounds(
                                    {other_one_step_x, other_one_step_y},
                                    start_x, start_y, start_x + width, start_y + height))) {
                            (**j)->takeMovedStepOne(input);
                            std::tie(other_one_step_x, other_one_step_y) = (**j)->getMovedStepOne(input);
                        }
                        (**j)->setHasMovedFlag(true);
                        if (solid_borders) {
                            if ((**j)->has_collider()) {
                                (**j)->getColliderComponent()->setCollisionStatus(true);
                                (**j)->getColliderComponent()->doAction();
                            }
                            collision = true;
                        }
                    } else {
                        (**j)->update(input);
                        (**j)->setHasMovedFlag(true);
                    }
                } else {
                    // case where entity j is not a player
                    (**j)->update(input);
                    (**j)->setHasMovedFlag(true);
                    if (solid_borders) {
                        if ((**j)->getShape().isOutOfBounds(start_x, start_y, start_x + width, start_y + height)) {
                            if ((**j)->has_collider()) {
                                (**j)->getColliderComponent()->setCollisionStatus(true);
                                (**j)->getColliderComponent()->doAction();
                            }
                            (**j)->setOffscreen(true);
                            collision = true;
                        }
                    } else {
                        if ((**j)->getShape().isOutOfBounds(start_x, start_y, start_x + width, start_y + height)) {
                            (**j)->setOffscreen(true);
                        } else {
                            (**j)->setOffscreen(false);
                        }
                    }
                }
            }
        }
        // If no collision detected and entity wasn't updated, call update now
        if (!collision && !(**i)->getHasMovedFlag()) {
            (**i)->update(input);
            (**i)->setHasMovedFlag(true);
        }
        if ((**i)->getShape().isOutOfBounds(start_x, start_y, start_x + width, start_y + height) && !solid_borders) {
            if ((**i)->getOffscreen()) {
                (**i)->incrementOffscreenCounter();
            } else {
                (**i)->setOffscreen(true);
                (**i)->resetOffscreenCounter();
            }
            if ((**i)->getOffscreenCounter() > (**i)->getMaxOffscreenAllowed()) {
                toRemove.push_back(**i);
            }
        } else if ((**i)->getOffscreen() && !(**i)->getShape().isOutOfBounds(start_x, start_y, start_x + width, start_y + height)) {
            (**i)->setOffscreen(false);
            (**i)->resetOffscreenCounter();
        }
    }
    for (auto& entity_ptr : toRemove) {
        removeEntity(entity_ptr);
    }
}

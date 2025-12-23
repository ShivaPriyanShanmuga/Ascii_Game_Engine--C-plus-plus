export module entity;

import <utility>; // done
import <vector>; // done
import <optional>; // done
import move_strategy; // done
import ncurses_win; // done
import shape; // done
import cyclic_transformations;
import abstract_collider;
import linear_move_strategy;
import player_movement_strat;

export class Entity{
    MoveStrategy* moveStrategy;
    Shape shape;
    Shape originalShape; // to reset to original shape if needed(remember to set the coordinates properly before resetting)
    std::optional<AbstractCollider*> colliderComponent;
    bool is_player = false;
    bool is_offscreen = false;
    int offscreen_counter = 0;
    int max_offscreen_counter_allowed = 10;
    bool has_moved = false;
    // the below are named badly, they actually represent the total steps to move from the original position in the current tick
    int steps_moved_x;
    int steps_moved_y;

public:
    Entity(Shape shp, MoveStrategy* strategy = nullptr, bool player = false, int max_offscreen_allowed = 10) : moveStrategy(strategy), shape(shp), originalShape(shp), is_player(player), max_offscreen_counter_allowed(max_offscreen_allowed), steps_moved_x(0), steps_moved_y(0) {}
   
    virtual ~Entity();

    std::pair<int, int> getPosition() const; // returns (x, y)

    void setPosition(int x, int y); // sets position to (x, y)

    void update(int input); // takes input and updates the entity's position based on its movement strategy

    void addMoveStrategy(MoveStrategy* strategy); // adds a movement strategy to the entity

    std::pair<int, int> getNextPosition(int input); // returns the next position based on current strategy and input

    void render(Window& win, Shape& shape) const; // render the given shape on the given window

    Shape& getShape(); // returns a reference to the shape of the entity

    void removeCyclicTransformationStrategies(); // removes cyclic strategies if any

    void resetMoveStrategies(); // resets movement strategies to original state

    std::pair<int, int> getOneStep(int input); // returns the next position after one step according to the current strategy

    void takeOneStep(int input); // moves the entity one step according to the current strategy

    void addColliderComponent(AbstractCollider* collider); // adds a collider component to the entity

    AbstractCollider* getColliderComponent(); // returns a pointer to the collider component if it exists

    bool has_collider(); // returns true if the entity has a collider component

    void resetColliderStatus(); // resets the collider status if it has a collider component

    bool isPlayer(); // returns true if the entity is a player

    void removeNonSpecialStrategies(); // removes all movement strategies except player control and cyclic strategies

    virtual void hasCollided() = 0; // called when a collision is detected and the user can modify the stats accordingly

    void setOffscreen(bool status); // sets the offscreen status

    bool getOffscreen() const; // gets the offscreen status

    void incrementOffscreenCounter(); // increments the offscreen counter by 1

    int getOffscreenCounter() const; // returns the current offscreen counter

    void resetOffscreenCounter(); // resets the offscreen counter to 0

    bool getHasMovedFlag(); // returns true if the entity has already moved in the current tick

    void setHasMovedFlag(bool status); // sets the has_moved flag

    void incrementStepsMoved(int dx, int dy); // increments the steps moved by dx and dy

    std::pair<int, int> getStepsMoved() const; // returns the steps to move

    void resetStepsMoved(); // resets the steps moved to zero

    int getMaxOffscreenAllowed() const; // returns the maximum allowed offscreen counter

    std::pair<int, int> getMovedStepOne(int input); // returns the step of size one using the steps moved only to be called after it has moved in the tick

    void takeMovedStepOne(int input); // takes a step of size one using the steps moved only to be called after it has moved in the tick
};

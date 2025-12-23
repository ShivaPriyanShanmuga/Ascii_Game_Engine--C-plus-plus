import game_engine;
import abstract_game;
import model;
import entity;
import shape;
import gravity_movement;
import linear_move_strategy;
import player_movement_strat;
import cyclic_transformations;
import move_strategy;
import <utility>;
import <vector>;
import <optional>;
import <memory>;
import abstract_collider;
import collider;
import abstract_input;
import inputkeyboard;

// this is a simple game to simulate a bouncing ball

class game : public AbstractGame {
    public:
        game() {}
        ~game() = default;
        void onTick(long long ticks, int input) override {}
};

class bounce : public Entity {
    public:
        bounce(int x, int y, int z = 0, char c = 'O') : Entity(Shape(x, y, z, c), nullptr, false, 100) {
            MoveStrategy* linear_strat = new LinearMoveStrategy(getShape(), nullptr, std::pair<int, int>{0, 1});
            addMoveStrategy(linear_strat);
            AbstractCollider* colliderComp = new Collider(*this);
            addColliderComponent(colliderComp);
        }
        ~bounce() {}
        void hasCollided() override {
            if (getPosition().second >= 21) {
                // reverse vertical direction
                resetMoveStrategies();
                MoveStrategy* linear_strat = new LinearMoveStrategy(getShape(), nullptr, std::pair<int, int>{0, -1});
                addMoveStrategy(linear_strat);
            } else if (getPosition().second <= 0) {
                // reverse vertical direction
                resetMoveStrategies();
                MoveStrategy* linear_strat = new LinearMoveStrategy(getShape(), nullptr, std::pair<int, int>{0, 1});
                addMoveStrategy(linear_strat);
            }
        }
};

int main() {
    Model model(22, 80, 0, 0, true); // assuming an 80x24 terminal size
    GameEngine engine(model);
    game g = game();
    Entity* ball = new bounce(40, 1, 0, 'O');
    model.addEntity(std::make_unique<Entity*>(ball));
    engine.run(g);
    return 0;
}



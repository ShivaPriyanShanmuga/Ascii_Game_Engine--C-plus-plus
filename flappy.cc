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

bool engine_running = true;


class pillar : public Entity {
    int bird_x;
public:
    pillar(int x, int y, int bird_x_pos, int z = 0, char c = '#', int height = 5, int width = 2, int speed = 1): Entity(Shape(x, y, z, width, height, c), nullptr, false, 100), bird_x(bird_x_pos) {
        // Set movement strategy after Entity is constructed
        // Create movement strategy with the velocity for leftward movement
        MoveStrategy* linearStrat = new LinearMoveStrategy(getShape(), nullptr, std::pair<int, int>{-speed, 0});
        addMoveStrategy(linearStrat);
    }
    ~pillar() {};
    void hasCollided() override {
        // On collision, stop the game
        engine_running = false;
    }
};

// i'm forward declaring these so that I can use them in game class
void add_pillar_collider(pillar *p) {
    AbstractCollider* colliderComp = new Collider(*p);
    p->addColliderComponent(colliderComp);
}

class Game: public AbstractGame {
    Model& model;
    GameEngine& engine;
    int pillar_spawn_interval = 50; // spawn a pillar every 50 ticks
public:
    Game(Model& m, GameEngine& e, int interval) : model(m), engine(e) , pillar_spawn_interval(interval) {}
    ~Game() {}
    void onTick(long long ticks, int input) override {
        if (!engine_running) {
            engine.stop();
        } else {
            if (ticks % pillar_spawn_interval == 0) {
                // Spawn a new pillar at the right edge of the screen with random height
                int pillar_height = 5 + (std::rand() % 10); // random height between 5 and 15
                Entity* new_pillar_top = new pillar(model.getWidth() - 3, model.getHeight() - pillar_height - 1, 10, 0, '#', pillar_height, 2, 1);
                add_pillar_collider(static_cast<pillar*>(new_pillar_top));
                model.addEntity(std::make_unique<Entity*>(new_pillar_top));
                Entity* new_pillar_bottom = new pillar(model.getWidth() - 3, 0, 10, 0, '#', model.getHeight() - pillar_height - 6, 2, 1);
                add_pillar_collider(static_cast<pillar*>(new_pillar_bottom));
                model.addEntity(std::make_unique<Entity*>(new_pillar_bottom));
            }
        }
    }
};


MoveStrategy* bird_movement_strategy(Shape& birdShape) {
    // Gravity strategy to pull the bird down
    MoveStrategy* gravityStrat = new GravityMoveStrategy(birdShape, nullptr, std::pair<int, int>{0, 1}, 1);
    // player movement strategy to allow user control
    MoveStrategy* playerStrat = new PlayerMovementStrategy(birdShape, gravityStrat, std::nullopt, {
        {(int)'w', {0, -2}}, // flap up
        {(int)'s', {0, 1}},   // move down faster
    });
    return playerStrat;
}


class bird : public Entity {
public:
    bird(int x, int y, int z = 0, char c = 'E') 
        : Entity(Shape(x, y, z, c), nullptr, true) {
        // Set movement strategy after Entity is constructed
        addMoveStrategy(bird_movement_strategy(getShape()));
    }
    ~bird() {};
    void hasCollided() {
        // On collision, stop the game
        engine_running = false;
    }
};

void add_bird_collider(bird *b) {
    AbstractCollider* colliderComp = new Collider(*b);
    b->addColliderComponent(colliderComp);
}



int main() {
    Model model(22, 80, 0, 0, false);
    GameEngine engine(model);
    Game game(model, engine, 40);

    // Create bird entity at position (10, 10)
    Entity* myBird = new bird(10, 10);
    add_bird_collider(static_cast<bird*>(myBird));
    model.addEntity(std::make_unique<Entity*>(myBird));
    engine.run(game);
    return 0;
}

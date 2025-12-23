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
import status_view;


bool engine_running = true;


class lazer : public Entity {
    Model& mod;
public:
    lazer(int x, int y, Model& model, int z = 1, char c = '|', std::pair<int, int> velocity = std::pair<int, int>{0, -1}): Entity(Shape(x, y, z, c), nullptr, false), mod(model) {
        MoveStrategy* linearStrat = new LinearMoveStrategy(getShape(), nullptr, velocity);
        addMoveStrategy(linearStrat);
        AbstractCollider* colliderComp = new Collider(*this);
        addColliderComponent(colliderComp);
        mod.addEntity(std::make_unique<Entity*>(this));
    }
    ~lazer() {};
    void hasCollided() override {
        mod.removeEntity(this);
    }
};


class enemy : public Entity {
    Model& mod;
    int* health;
    std::vector<Shape> movementShapes;
public:
    enemy(int x, int y, Model& model, int z = 1): 
        Entity(Shape(x, y, z, std::vector<std::pair<std::pair<int, int>, char>>{
            {{0,0}, 'E'}, {{1,0}, 'E'}, {{2,0}, 'E'},
            {{0,1}, 'E'}, {{1,1}, 'X'}, {{2,1}, 'E'},
            {{0,2}, 'E'}, {{1,2}, 'E'}, {{2,2}, 'E'}
        }), nullptr, false), 
        mod(model), 
        health(new int(2)) {
        
        // Create cyclic transformation shapes for left-to-right movement
        
        // Create shapes to simulate movement with 3-space gaps going left to right
        for (int step = 0; step < 5; ++step) {
            std::vector<std::pair<std::pair<int, int>, char>> bitmap = {
                {{step * 3, 0}, 'E'}, {{step * 3 + 1, 0}, 'E'}, {{step * 3 + 2, 0}, 'E'},
                {{step * 3, 1}, 'E'}, {{step * 3 + 1, 1}, 'X'}, {{step * 3 + 2, 1}, 'E'},
                {{step * 3, 2}, 'E'}, {{step * 3 + 1, 2}, 'E'}, {{step * 3 + 2, 2}, 'E'}
            };
            movementShapes.push_back(Shape(x, y, z, bitmap));
        }
        
        // Create shapes to simulate movement going right to left
        for (int step = 3; step >= 1; --step) {
            std::vector<std::pair<std::pair<int, int>, char>> bitmap = {
                {{step * 3, 0}, 'E'}, {{step * 3 + 1, 0}, 'E'}, {{step * 3 + 2, 0}, 'E'},
                {{step * 3, 1}, 'E'}, {{step * 3 + 1, 1}, 'X'}, {{step * 3 + 2, 1}, 'E'},
                {{step * 3, 2}, 'E'}, {{step * 3 + 1, 2}, 'E'}, {{step * 3 + 2, 2}, 'E'}
            };
            movementShapes.push_back(Shape(x, y, z, bitmap));
        }
        
        // Add cyclic transformation strategy for movement
        MoveStrategy* cyclicStrat = new CyclicTransformation(getShape(), movementShapes, 15);
        addMoveStrategy(cyclicStrat);
        
        // Add collider
        AbstractCollider* colliderComp = new Collider(*this);
        addColliderComponent(colliderComp);
        
        mod.addEntity(std::make_unique<Entity*>(this));
    }
    
    ~enemy() {
        delete health;
    };
    
    int* getHealth() {
        return health;
    }
    
    void hasCollided() override {
        (*health) -= 1;
        if ((*health) <= 0) {
            mod.removeEntity(this);
        }
    }
};


class Game : public AbstractGame {
    Model& model;
    GameEngine& engine;
    bool has_won = false;
    int* player_hlth;
    Shape* player_shape = nullptr;
    StatusViewer* status_view = nullptr;
    long long win_tick = -1;
public:
    Game(Model& m, GameEngine& e) : model(m), engine(e) {}
    ~Game() {}
    Model& getModel()  {
        return model;
    }
    GameEngine& getEngine()  {
        return engine;
    }
    bool checkWinCondition() const {
        return has_won;
    }

    void setWinCondition(bool won) {
        has_won = won;
    }

    int countEnemies() const {
        int enemy_count = 0;
        for (const auto& entity_ptr : model.getEntities()) {
            Entity* entity = *entity_ptr;
            // Count enemies: entities that are not players
            if (!entity->isPlayer()) {
                // Check if it has 'X' in the shape (enemy marker)
                const auto& charmap = entity->getShape().getCharMap();
                for (const auto& [pos, ch] : charmap) {
                    if (ch == 'X') {
                        enemy_count++;
                        break; // Count this entity once
                    }
                }
            }
        }
        return enemy_count;
    }

    void displayMessage()  {
        engine.status_render(std::string("Health: ") + std::to_string(*player_hlth));
        int enemy_count = countEnemies();
        engine.status_render(std::string("Enemies: ") + std::to_string(enemy_count));
        
        if (enemy_count == 0 && !has_won) {
            has_won = true;
            win_tick = engine.getElapsedTicks();
        }
        
        if (has_won) {
            engine.status_render(std::string("You Won!"));
            // Exit after 5 seconds (assuming ~60 ticks per second, 300 ticks = 5 seconds)
            if (engine.getElapsedTicks() - win_tick > 300) {
                engine_running = false;
            }
        }
    }

    void setHealth(int* health) {
        player_hlth = health;
    }

    void setPlayerShape(Shape& shape) {
        player_shape = &shape;
    }

    void setStatusView(StatusViewer& sv) {
        status_view = &sv;
    }

    void onTick(long long ticks, int input) override {
        if (status_view != nullptr) {
            status_view->setCurrentTick(ticks);
        }
        if (!engine_running) {
            engine.stop();
        } else {
            if (input == ' ' && player_shape != nullptr) {
                // Spawn a lazer from player's current position
                int lazer_x = player_shape->getX();
                int lazer_y = player_shape->getY() - 1; // just above the player
                new lazer(lazer_x, lazer_y, model, 1, '|', {0, -1});
            }
            displayMessage();
        }
    }
};

class interface {
    Game& game;
public:
    interface(Game& g) : game(g) {}
    ~interface() {}
    void setWin(bool won) {
        game.setWinCondition(won);
    }

    void setHealth(int* health) {
        game.setHealth(health);
    }
};

class player : public Entity {
    Model& model;
    interface& game_interface;
    int* health = new int(3);
public:
    player(int x, int y,  Model& mod, interface& inter, std::vector<std::pair<std::pair<int, int>, char>> bitmap, int z = 0): Entity(Shape(x, y, z, bitmap), nullptr, true), model(mod), game_interface(inter) {
        // Set movement strategy after Entity is constructed
        game_interface.setHealth(health);
        addMoveStrategy(new PlayerMovementStrategy(getShape(), nullptr, std::nullopt, {
            {(int)'a', {-1, 0}}, // move left
            {(int)'d', {1, 0}},  // move right
        }));
        AbstractCollider* colliderComp = new Collider(*this);
        addColliderComponent(colliderComp);
        model.addEntity(std::make_unique<Entity*>(this));
    }

    ~player() {
        delete health;
    };

    int* getHealth() {
        return health;
    }

    void hasCollided() override {
        (*health) -= 1;
        if ((*health) <= 0) {
            // End the game
            engine_running = false;
        }
    }
};


int main() {
    Model model{22, 80, 0, 0, false};
    GameEngine engine(model);
    Game game(model, engine);
    interface game_interface(game);
    player* p1 = new player (39, 17, model, game_interface, {{ {0,0}, '=' }, { {-1,0}, '<' }, { {1,0}, '>' }}, 1);
    game.setPlayerShape(p1->getShape());
    
    // Create back row of enemies (higher y position)
    for (int i = 0; i < 4; ++i) {
        int enemy_x = 5 + (i * 15);  // 2-space gap between enemies (15 = 'E' char + 2 gap + next starting position)
        new enemy(enemy_x, 3, model, 1);
    }
    
    // Create front row of enemies (lower y position)
    for (int i = 0; i < 4; ++i) {
        int enemy_x = 5 + (i * 15);  // 2-space gap between enemies
        new enemy(enemy_x, 10, model, 1);
    }
    
    engine.run(game);
    return 0;
}

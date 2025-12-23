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
import inputkeyboard;import game_engine;
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
import <unordered_map>;
import abstract_collider;
import collider;
import abstract_input;
import inputkeyboard;

bool engine_running = true;
Model model;
GameEngine engine(model);


class Player_Ship : public Entity {
    int health = 3;
    bool is_blinking = false;
    int blink_count = 0;
    std::vector<Shape> blink_shapes = {
        Shape(getShape().getX(), getShape().getY(), getShape().getZ(), std::vector<std::pair<std::pair<int, int>, char>>{{ {0,0}, ' ' }}), // invisible
        getShape() // original
    };
public:
    Player_Ship(int x, int y, int z = 0, std::vector<std::pair<std::pair<int, int>, char>> charmap = {{ {0,0}, 'A' }}): Entity(Shape(x, y, z, charmap), nullptr, true) {
        addMoveStrategy(new PlayerMovementStrategy(getShape(), nullptr, std::nullopt, {
            {(int)'a', {-1, 0}}, // move left
            {(int)'d', {1, 0}},  // move right
            {(int)'w', {0, -1}}, // move up
            {(int)'s', {0, 1}},   // move down
        }));
    }
    ~Player_Ship() {};

    int getHealth() const {
        return health;
    }

    bool isBlinking() const {
        return is_blinking;
    }

    int& getBlinkCount() {
        return blink_count;
    }

    const int& getBlinkCount() {
        return blink_count;
    }

    Shape& getBlinkShape() const {
        if (is_blinking){
            return blink_shapes[blink_count % blink_shapes.size()];
        }else {
            return getShape();
        }
    }

    void hasCollided() override {
        health--;
        if (health <= 0) {
            engine_running = false; // End the game
        } else {
            if (!is_blinking) {
                is_blinking = true;
                addMoveStrategy(new CyclicTransformation(getShape(), blink_shapes, nullptr));
            } else {
                blink_count++;
            }
        }
    }
};

class Enemy_Ship;
class lazer;

class Game: public AbstractGame {
    Model& model;
    GameEngine& engine;
    int lazer_spawn_interval = 20; // spawn a lazer every 20 ticks
    std::vector<Enemy_Ship*> frontline_enemies;
    std::vector<Enemy_Ship*> backline_enemies;
    Player_Ship* player_ship;
    bool has_won = false;
public:
    Game(Model& m, GameEngine& e, int interval) : model(m), engine(e) , lazer_spawn_interval(interval), player_ship(nullptr) {}
    ~Game() {}
    bool noEnemiesLeft() {
        bool frontline_empty = false;
        bool backline_empty = false;
        for (auto& enemy : frontline_enemies) {
            if (enemy == nullptr) {
                frontline_empty = true;
            } else {
                frontline_empty = false;
                break;
            }
        }
        for (auto& enemy : backline_enemies) {
            if (enemy == nullptr) {
                backline_empty = true;
            } else {
                backline_empty = false;
                break;
            }
        }
        return frontline_empty && backline_empty;
    }

    void addPlayerShip(Player_Ship* p) {
        player_ship = p;
    }

    std::vector<Enemy_Ship*>& getFrontlineEnemies() {
        return frontline_enemies;
    }

    std::vector<Enemy_Ship*>& getBacklineEnemies() {
        return backline_enemies;
    }

    void setEnemyToNullptr(Enemy_Ship* enemy) {
        for (size_t i = 0; i < frontline_enemies.size(); ++i) {
            if (frontline_enemies[i] == enemy) {
                frontline_enemies[i] = nullptr;
                return;
            }
        }
        for (size_t i = 0; i < backline_enemies.size(); ++i) {
            if (backline_enemies[i] == enemy) {
                backline_enemies[i] = nullptr;
                return;
            }
        }
    }

    void onTick(long long ticks, int input) override {
        if (!engine_running) {
            engine.stop();
        } else {
            if (noEnemiesLeft()) {
                has_won = true;
                engine.stop();
            } else if (player_ship->getHealth() <= 0) {
                engine_running = false;
                engine.stop();
            } else {
                if (ticks % lazer_spawn_interval == 0) {
                    int randomenemy_index = rand() % frontline_enemies.size();
                    Enemy_Ship* target_enemy = frontline_enemies[randomenemy_index];
                    if (target_enemy != nullptr) {
                        lazer* new_lazer = new lazer(target_enemy->getShape().getX(), target_enemy->getShape().getY() + 1, 0, '|');
                        AbstractCollider* colliderComp = new Collider(*new_lazer);
                        new_lazer->addColliderComponent(colliderComp);
                        engine.spawnEntity(new_lazer);
                    } else {
                        target_enemy = backline_enemies[randomenemy_index];
                        if (target_enemy != nullptr) {
                            lazer* new_lazer = new lazer(target_enemy->getShape().getX(), target_enemy->getShape().getY() + 1, 0, '|');
                            AbstractCollider* colliderComp = new Collider(*new_lazer);
                            new_lazer->addColliderComponent(colliderComp);
                            engine.spawnEntity(new_lazer);
                        }
                    }
                }
                if (player_ship == nullptr) {
                    if (input == ' ') {
                        // shoots a lazer
                        lazer* new_lazer = new lazer(player_ship->getShape().getX(), player_ship->getShape().getY() - 1, 0, '|');
                        AbstractCollider* colliderComp = new Collider(*new_lazer);
                        new_lazer->addColliderComponent(colliderComp);
                        engine.spawnEntity(new_lazer);
                    }
                }
            }
        }
        if (player_ship->isBlinking()) {
            player_ship->getBlinkCount()++;
            if (player_ship->getBlinkCount() >= 4) {
                player_ship->isBlinking() = false;
                player_ship->getBlinkCount() = 0;
                player_ship->removeCyclicTransformationStrategies();
            }
        }
    }
};

Game game(model, engine, 20);

class lazer : public Entity {
public:
    lazer(int x, int y, int z = 0, char c = '|'): Entity(Shape(x, y, z, c), nullptr, false) {
        // Lazer specific initialization can go here
    }
    ~lazer() {};
    void hasCollided() override {
        model.removeEntity(this);
    }
};



class Enemy_Ship : public Entity {
    int health = 1;
    public:
    Enemy_Ship(int x, int y, int z = 0, char c = 'W'): Entity(Shape(x, y, z, c), nullptr, false) {
        addMoveStrategy(new CyclicTransformation(getShape(), {
            Shape(getShape().getX(), getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 2, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 4, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 6, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 8, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 10, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 12, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 14, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 16, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 18, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 20, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 22, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 24, getShape().getY(), getShape().getZ(), c),
            Shape(getShape().getX() + 26, getShape().getY(), getShape().getZ(), c),
        }, nullptr));
    }
    ~Enemy_Ship() {};
    void hasCollided() override {
        health--;
        if (health <= 0) {
            game.setEnemyToNullptr(this);
            model.removeEntity(this);
        }
    }
};

int main() {
    std::vector<Enemy_Ship*>& frontline = game.getFrontlineEnemies();
    std::vector<Enemy_Ship*>& backline = game.getBacklineEnemies(); 
    for (int i = 0; i < 5; ++i) {
        Enemy_Ship* enemy1 = new Enemy_Ship(5 + i * 10, 2, 0, 'W');
        AbstractCollider* colliderComp1 = new Collider(*enemy1);
        enemy1->addColliderComponent(colliderComp1);
        engine.spawnEntity(enemy1);
        frontline.push_back(enemy1);

        Enemy_Ship* enemy2 = new Enemy_Ship(5 + i * 10, 5, 0, 'M');
        AbstractCollider* colliderComp2 = new Collider(*enemy2);
        enemy2->addColliderComponent(colliderComp2);
        engine.spawnEntity(enemy2);
        backline.push_back(enemy2);
    }
    Player_Ship* player_ship = new Player_Ship(40, 20, 0, {{ {0,0}, 'A' }, { {1,0}, '>' }, { {2,0}, '<' }});
    AbstractCollider* playerColliderComp = new Collider(*player_ship);
    player_ship->addColliderComponent(playerColliderComp);
    engine.spawnEntity(player_ship);

    engine.run(game);
    return 0;
}


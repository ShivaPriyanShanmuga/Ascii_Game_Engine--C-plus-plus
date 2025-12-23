export module game_engine;

import clock;
import model;
import ncurses_win;
import game_view;
import status_view;
import ncurses_scr;
import <memory>;
import abstract_game;
import abstract_input;
import inputkeyboard;
import entity;



export class GameEngine {
    Clock clock;
    Model& model;
    Screen screen;
    GameViewer view;
    StatusViewer statusView;
    bool isRunning = true;
    AbstractInput* inputHandler;
public:
    GameEngine(Model& mod): model(mod), view(screen.AddWindow(22, 80, 0, 0)), statusView(screen.AddWindow(3, 80, 22, 0)), inputHandler(new InputKeyboard(view.getWindow())) {
        view.getWindow().enableKeypad(true);
        view.getWindow().enableNonBlockingInput();
    }

    ~GameEngine();
    long long getElapsedTicks() const;
    void spawnEntity(Entity* entity);
    void removeEntity(Entity* entity);
    void run(AbstractGame& game);
    Model& getModel();
    void status_render(const std::string& status);
    void stop();
};

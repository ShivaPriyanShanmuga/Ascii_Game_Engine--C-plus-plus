module game_engine;

import clock; // done
import model; // done
import ncurses_win; 
import game_view; // done
import status_view; // done
import ncurses_scr; // done
import <memory>;
import abstract_game; // done
import abstract_input; // done
import inputkeyboard; // done
import entity; // done
import <string>;

GameEngine::~GameEngine() {
    delete inputHandler;
}

long long GameEngine::getElapsedTicks() const {
    return clock.get_tick();
}

void GameEngine::spawnEntity(Entity* entity) {
    std::unique_ptr<Entity*> entity_ptr = std::make_unique<Entity*>(entity);
    model.addEntity(std::move(entity_ptr));
}

void GameEngine::removeEntity(Entity* entity) {
    model.removeEntity(entity);
}

void GameEngine::run(AbstractGame& game) {
    while (isRunning) {
        clock.tick_forward();
        int tick = clock.get_tick();
        int input = inputHandler->get_input();
        model.updateEntities(input);
        view.clear();
        statusView.getWindow().clear();
        statusView.resetForNewTick();
        game.onTick(tick, input);
        view.renderEntities(model.getEntities());
        view.getWindow().addBorder('+', '+', '+', '+', '-', '|');
        view.getWindow().refresh();
        statusView.getWindow().refresh();
        if (game.isGameOver()) {
            isRunning = false;
        }
        model.resetEntitiesAfterTick();
        inputHandler->clear_input_buffer(); // to clear any pending inputs
    }
}

Model& GameEngine::getModel() { return model; }

void GameEngine::status_render(const std::string& status) {
    statusView.renderStatus(status);
    statusView.getWindow().refresh();
}

void GameEngine::stop() {
    isRunning = false;
}

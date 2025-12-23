module game_view;

import <vector>;
import ncurses_win;
import viewer;
import entity;
import <memory>;

void GameViewer::renderEntities(std::vector<std::unique_ptr<Entity*>>& entities) {
    Window& win = getWindow();
    win.clear();
    for (const auto& entity : entities) {
        (*entity)->render(win, (*entity)->getShape());
    }
    win.refresh();
}

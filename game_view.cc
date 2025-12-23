export module game_view;

import <vector>;
import ncurses_win;
import viewer;
import entity;
import <memory>;

export class GameViewer : public Viewer {
public:
    GameViewer(Window& window) : Viewer(window) {}
    ~GameViewer() = default;

    void renderEntities(std::vector<std::unique_ptr<Entity*>>& entities);  
};

export module status_view;

import <string>;
import viewer;
import ncurses_win;

export class StatusViewer : public Viewer {
    int status_line = 0;
    int position_col = 1;
    long long last_tick = -1;
public:
    StatusViewer(Window& window) : Viewer(window) {}
    ~StatusViewer() = default;
    void renderStatus(const std::string& status);
    void clearStatus();
    void resetForNewTick() {
        win.clear();
        status_line = 0;
    }
    void setCurrentTick(long long tick) {
        if (tick != last_tick) {
            win.clear();
            status_line = 0;
            last_tick = tick;
        }
    }
};

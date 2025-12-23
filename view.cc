export module viewer;

import <vector>;
import ncurses_win;


export class Viewer {
protected:
    Window& win; // it only uses the window but does not own it
public:
    Viewer(Window& window): win(window) {}
    ~Viewer() = default;
    Window& getWindow() { return win; }
    inline void clear() { win.clear(); }
    inline void refresh() { win.refresh(); }
};

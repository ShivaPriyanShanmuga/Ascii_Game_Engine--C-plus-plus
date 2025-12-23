module ncurses_scr;

import <vector>;
import ncurses_win;
import <memory>;

Screen::~Screen() {
    endwin();
}

void Screen::clearScreen() {
    clear();
}

void Screen::refreshScreen() {
    refresh();
}

Window& Screen::AddWindow(int height, int width, int starty, int startx) {
    windows.push_back(std::make_unique<Window>(height, width, starty, startx));
    return *(windows.back());
}

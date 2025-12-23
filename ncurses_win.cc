export module ncurses_win;

import <string>;
#include <ncurses.h>

export class Window {
    WINDOW* win;
    int height, width, starty, startx;
    bool border = false;
    int tlc = '+', trc = '+', blc = '+', brc = '+', hline = '-', vline = '|';
public:
    Window(int height, int width, int starty, int startx);
    ~Window();
    void refresh();
    void clear();
    void addBorder(char tlc, char trc, char blc, char brc, char hline, char vline);
    void enableKeypad(bool flag);
    void mvWaddch(int y, int x, char ch);
    int getMaxx();
    int getMaxy();
    void mvWprintw(int y, int x, const std::string& str);
    int get_input();
    void enableNonBlockingInput() {
        nodelay(win, TRUE);
    }
    void clearInputBuffer();
};

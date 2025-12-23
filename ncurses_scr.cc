export module ncurses_scr;

import <vector>;
import ncurses_win;
import <memory>;

#include <ncurses.h>


    export class Screen {
        // replace with window class
        std::vector<std::unique_ptr<Window>> windows;
        public:
            Screen() {
                initscr();
                cbreak();
                noecho();
                keypad(stdscr, true);
                curs_set(0); // hides cursor
                refresh();
            }
            ~Screen();
            void clearScreen();
            void refreshScreen();
            Window& AddWindow(int height, int width, int starty, int startx);
    };    

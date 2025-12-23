module status_view;

import <string>;
import viewer;
import ncurses_win;

#include <ncurses.h>

void StatusViewer::renderStatus(const std::string& status) {
    win.mvWprintw(status_line, position_col, status);
    status_line++;
}

void StatusViewer::clearStatus() {
    win.clear();
    status_line = 0;
    position_col = 1;
}

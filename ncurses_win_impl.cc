module ncurses_win;

import <string>;

Window::~Window() {
    delwin(win);
}
void Window::refresh() {
    wrefresh(win);
}
void Window::clear() {
    werase(win);
    if (border) {
        wborder(win, vline, vline, hline, hline, tlc, trc, blc, brc);
    }
    wrefresh(win);
}
void Window::addBorder(char tlc, char trc, char blc, char brc, char hline, char vline) {
    border = true;
    this->tlc = tlc;
    this->trc = trc;
    this->blc = blc;
    this->brc = brc;
    this->hline = hline;
    this->vline = vline;
    wborder(win, vline, vline, hline, hline, tlc, trc, blc, brc);
    wrefresh(win);
}
void Window::enableKeypad(bool flag) {
    keypad(win, flag);
}

void Window::mvWaddch(int y, int x, char ch) {
    mvwaddch(win, y, x, ch);
}

int Window::get_input() {
    int ch = wgetch(win);
    if (ch == -1) {
        return -1; // No input
    }
    return ch;
}

int Window::getMaxx() {
    return getmaxx(win);
}

int Window::getMaxy() {
    return getmaxy(win);
}

void Window::mvWprintw(int y, int x, const std::string& str) {
    mvwprintw(win, y, x, "%s", str.c_str());
}

Window::Window(int height, int width, int starty, int startx) : height(height), width(width), starty(starty), startx(startx) {
    win = newwin(height, width, starty, startx);
    if (border) {
        wborder(win, vline, vline, hline, hline, tlc, trc, blc, brc);
    }
    wrefresh(win);
}

void Window::clearInputBuffer() {
    flushinp();
}


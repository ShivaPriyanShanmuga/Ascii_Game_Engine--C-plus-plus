module inputkeyboard;

import abstract_input;
import ncurses_win;

int InputKeyboard::get_input() {
    return win.get_input();
}

void InputKeyboard::clear_input_buffer() {
    win.clearInputBuffer();
}

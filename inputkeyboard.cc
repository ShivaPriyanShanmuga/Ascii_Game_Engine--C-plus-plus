export module inputkeyboard;

import abstract_input;
import ncurses_win;

export class InputKeyboard : public AbstractInput {
public:
    InputKeyboard(Window& w) : AbstractInput(w) {}
    int get_input() override;
    void clear_input_buffer() override;
};

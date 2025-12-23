export module abstract_input;

import ncurses_win;

export class AbstractInput {
protected:
    Window& win;
public:
    AbstractInput(Window& w) : win(w) {}
    virtual ~AbstractInput() {}
    virtual int get_input() = 0;
    virtual void clear_input_buffer() {};
};

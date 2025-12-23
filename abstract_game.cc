export module abstract_game;


export class AbstractGame {
    bool isGameOverFlag = false;
public:
    AbstractGame() = default;
    virtual ~AbstractGame() = default;
    virtual void onTick(long long ticks, int input) = 0;
    bool isGameOver() const {
        return isGameOverFlag;
    }
    void setGameOver(bool status) {
        isGameOverFlag = status;
    }
};

export module clock;

export class Clock {
    long long tick;
    int tick_duration_ms = 50; // duration of each tick in milliseconds
public:
    Clock() : tick(0) {}
    void tick_forward();
    long long get_tick() const;
};

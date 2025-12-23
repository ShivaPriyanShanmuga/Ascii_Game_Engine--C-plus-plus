module clock;

import <chrono>;
import <thread>;

void Clock::tick_forward() {
    std::this_thread::sleep_for(std::chrono::milliseconds(tick_duration_ms));
    tick += 1;
}

long long Clock::get_tick() const {
    return tick;
}

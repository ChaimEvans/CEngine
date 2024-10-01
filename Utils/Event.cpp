#include "Event.h"

namespace CEngine {
    Event &Event::operator+=(const std::function<void()> &func) {
        functions.push_back(func);
        return *this;
    }

    void Event::Invoke() {
        for (const auto &item: functions) {
            item();
        }
    }
}

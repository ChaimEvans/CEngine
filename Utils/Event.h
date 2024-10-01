#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <functional>
#include <tuple>

namespace CEngine {
    class Event {
    public:
        Event() = default;
        ~Event() = default;

        Event &operator+=(const std::function<void()> &func);
        // Event& operator+=(std::tuple<void*(*)()> t);
        void Invoke();

    private:
        std::vector<std::function<void()> > functions;
    };
}
#endif //EVENT_H

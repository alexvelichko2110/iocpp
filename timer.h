#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:

    typedef std::chrono::system_clock::time_point time_point_type;

    Timer() {
        save_time_point();
    }

    time_point_type get_time_point() {
        return _time_point;
    }

    void save_time_point() {
        _time_point = std::chrono::system_clock::now();
    }

    time_point_type get_time_point_now() {
        return std::chrono::system_clock::now();
    }

    time_point_type get_time_point_experience (int miliseconds) {
        return std::chrono::milliseconds(miliseconds) + _time_point;
    }

private:

    time_point_type _time_point;
};

#endif // TIMER_H

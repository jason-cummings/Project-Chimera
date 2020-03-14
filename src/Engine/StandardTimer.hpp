// A very simple timer that uses SDL functions to get the time of the last tick

#ifndef STANDARDTIMER_H
#define STANDARDTIMER_H

#include <SDL.h>

#include "Timer.hpp"

class StandardTimer: public Timer {
private:
    Uint64 last_time, current_time, frequency;
public:
    StandardTimer();
    double getLastTickTime();
};

#endif
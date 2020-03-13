#include "StandardTimer.hpp"

StandardTimer::StandardTimer() {
    current_time = SDL_GetPerformanceCounter();
    last_time = current_time;
}

double StandardTimer::getLastTickTime() {
    current_time = SDL_GetPerformanceCounter();
    frequency = SDL_GetPerformanceFrequency();
    double dt = ((current_time - last_time) / (double)frequency);
    last_time = current_time;
    return dt;
}

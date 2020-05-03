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
    last_perf = current_time;
    return dt;
}

double StandardTimer::timePerformance() {
    Uint64 perf_cur_time = SDL_GetPerformanceCounter();
    Uint64 temp_frequency = SDL_GetPerformanceFrequency();
    double dt = ((perf_cur_time - last_perf) / (double)temp_frequency);
    last_perf = perf_cur_time;
    return dt;
}

#include "PerformanceLogger.hpp"

#include <iostream>

#define DEBUG
#define TICK_TIME_WARNING_THRESHOLD .2
#define PRINT_FPS true

void PerformanceLogger::startTick() {
	operations.clear();
	measurements.clear();
	total_tick_time = 0.0;
}

void PerformanceLogger::addOperation(std::string operation, double measurement) {
	operations.push_back(operation);
	measurements.push_back(measurement);
	total_tick_time += measurement;
}

void PerformanceLogger::stopTick() {
	time_since_FPS += total_tick_time;
	frames++;

	#ifdef DEBUG
	if(total_tick_time > TICK_TIME_WARNING_THRESHOLD) {
		std::cout << "WARNING: Long Tick: " << total_tick_time << " seconds" << std::endl;
		std::cout << "Current FPS: " << frames / time_since_FPS << std::endl;

		for(int i = 0; i < operations.size(); i++) {
			std::cout << " - " << operations[i] << ": " << measurements[i] << std::endl;
		}
	}


	if(time_since_FPS > 5.0) {
	// #ifdef PRINT_FPS
		std::cout << "FPS: " << frames / time_since_FPS << std::endl;
	// #endif
		time_since_FPS = 0.0;
		frames = 0;
	}
	#endif
}
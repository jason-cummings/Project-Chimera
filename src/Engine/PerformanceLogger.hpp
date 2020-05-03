#ifndef PERFORMANCE_LOGGER_H
#define PERFORMANCE_LOGGER_H

#include <string>
#include <iostream>
#include <vector>

#define TICK_TIME_WARNING_THRESHOLD .033

#define PRINT_FPS true

class PerformanceLogger {
	int frames;
	double time_since_FPS;

	std::vector<std::string> operations;
	std::vector<double> measurements;
	double total_tick_time;

public:
	PerformanceLogger() {frames = 0; time_since_FPS = 0.0;}
	void startTick();

	void addOperation(std::string operation, double measurement);

	void stopTick();
};

#endif
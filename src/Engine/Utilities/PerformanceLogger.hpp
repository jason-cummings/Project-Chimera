#ifndef PERFORMANCE_LOGGER_H
#define PERFORMANCE_LOGGER_H

#include <string>
#include <vector>

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
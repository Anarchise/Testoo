#pragma once
#include <string>

class Timer
{
public:
	static uint64_t time();
public:
	Timer();
	Timer(uint32_t threshold);
	Timer(bool autoStart, uint32_t threshold);
	void start();
	void stop();
	void reset();
	bool passedThreshold();
	uint64_t since();
	uint64_t sinceAndReset();
public:
	uint32_t Threshold;
private:
	uint64_t m_start;
};

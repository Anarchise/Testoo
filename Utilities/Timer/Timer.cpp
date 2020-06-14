//Timer.cpp
#include "Timer.h"
#include <ctime>

uint64_t Timer::time()
{
	return (uint64_t)std::time(nullptr);
}

Timer::Timer()
{
	this->m_start = 0;
	this->Threshold = 0;
}

Timer::Timer(uint32_t threshold)
{
	this->m_start = 0;
	this->Threshold = threshold;
}

Timer::Timer(bool autoStart, uint32_t threshold)
{
	this->m_start = 0;
	this->Threshold = threshold;

	if (autoStart)
		this->start();
}

void Timer::start()
{
	this->m_start = Timer::time();
}

void Timer::stop()
{
	this->m_start = 0;
}

void Timer::reset()
{
	this->start();
}

bool Timer::passedThreshold()
{
	if (this->m_start == 0)
		return false;
	return this->since() > this->Threshold;
}

uint64_t Timer::since()
{
	return Timer::time() - this->m_start;
}

uint64_t Timer::sinceAndReset()
{
	uint64_t timeSince = this->since();
	this->reset();
	return timeSince;
}

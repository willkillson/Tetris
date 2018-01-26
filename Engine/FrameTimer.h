#pragma once
#ifndef FRAMETIMER_H
#define FRAMETIMER_H
#include <chrono>

class FrameTimer
{

public:
	FrameTimer();
	float Mark();

private:
	std::chrono::steady_clock::time_point last;
};

#endif
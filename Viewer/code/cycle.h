#pragma once

#include <vector>
#include "timer.h"

class Cycle
{
public:
	Cycle(std::initializer_list<float> times);

	void update(float delta);
	bool isActive(int cycle);

	float getRatio()
	{
		return this->timers[this->activeTimer].getRatio();
	}

	int activeTimer = 0;
	std::vector<Timer> timers;

	void incrementTimer();
};

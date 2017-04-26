#include "cycle.h"

Cycle::Cycle(std::initializer_list<float> times)
{
	for (float time : times)
	{
		this->timers.emplace_back(time);
	}
}

void Cycle::update(float delta)
{
	this->timers[this->activeTimer].update(delta);
	if (this->timers[this->activeTimer].resetIfReady())
	{
		this->incrementTimer();
	}
}

bool Cycle::isActive(int cycle)
{
	return this->activeTimer == cycle;
}

void Cycle::incrementTimer()
{
	this->activeTimer = (this->activeTimer + 1) % this->timers.size();
}

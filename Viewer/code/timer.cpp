#include "timer.h"

Timer::Timer(float cooldownTime) : cooldownTime(cooldownTime), elapsedTime(0.0f)
{

}

void Timer::update(float delta)
{
	this->elapsedTime += delta;
}

bool Timer::isReady()
{
	return this->elapsedTime >= this->cooldownTime;
}
bool Timer::resetIfReady()
{
	if (this->isReady())
	{
		this->reset();
		return true;
	}
	else return false;
}
void Timer::reset()
{
	this->elapsedTime = 0.0f;
}

float Timer::getElapsed()
{
	return this->elapsedTime;
}
float Timer::getRatio()
{
	return this->elapsedTime / this->cooldownTime;
}

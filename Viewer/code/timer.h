#pragma once

class Timer
{
public:
	Timer(float cooldownTime);

	void update(float delta);

	bool isReady();
	bool resetIfReady();
	void reset();

	float getElapsed();
	float getRatio();

private:
	float elapsedTime;
	float cooldownTime;
};

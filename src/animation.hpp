#pragma once

#include "kinematicChain.hpp"

#include <glm/glm.hpp>

#include <chrono>
#include <functional>
#include <vector>

class Animation
{
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock,
		std::chrono::duration<float, std::nano>>;

public:
	Animation(const std::function<void(void)>& updateCurrChain);
	void start();
	void stop();
	void reset();
	void update();
	float getTime() const;
	float getEndTime() const;
	void setEndTime(float time);

private:
	std::function<void(void)> m_updateCurrChain{};
	TimePoint m_startTime{};
	float m_currentTime = 0;
	float m_endTime = 5;
	bool m_running = false;

	static TimePoint now();
};

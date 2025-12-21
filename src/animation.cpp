#include "animation.hpp"

#include <glm/gtc/constants.hpp>

#include <algorithm>
#include <cmath>
#include <cstddef>

Animation::Animation(const std::function<void(void)>& updateCurrChain) :
	m_updateCurrChain{updateCurrChain}
{ }

void Animation::start()
{
	if (m_running)
	{
		return;
	}

	m_startTime = now() - std::chrono::duration<float>(m_currentTime);
	m_running = true;
}

void Animation::stop()
{
	m_running = false;
}

void Animation::reset()
{
	stop();
	m_currentTime = 0;
	m_updateCurrChain();
}

void Animation::update()
{
	if (!m_running)
	{
		return;
	}

	m_currentTime =
		std::chrono::duration_cast<std::chrono::duration<float>>(now() - m_startTime).count();
	if (m_currentTime >= m_endTime)
	{
		m_currentTime = m_endTime;
		m_running = false;
	}
	m_updateCurrChain();
}

float Animation::getTime() const
{
	return m_currentTime;
}

float Animation::getEndTime() const
{
	return m_endTime;
}

void Animation::setEndTime(float time)
{
	m_endTime = time;
	m_currentTime = std::min(m_currentTime, time);
	m_updateCurrChain();
}

Animation::TimePoint Animation::now()
{
	return std::chrono::high_resolution_clock::now();
}

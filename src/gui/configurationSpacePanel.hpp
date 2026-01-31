#pragma once

#include "scene.hpp"

#include <glm/glm.hpp>

class ConfigurationSpacePanel
{
public:
	static constexpr int size = 376;

	ConfigurationSpacePanel(const Scene& scene, const glm::ivec2& viewportSize);
	void update();

private:
	const Scene& m_scene;
	const glm::ivec2& m_viewportSize;
};

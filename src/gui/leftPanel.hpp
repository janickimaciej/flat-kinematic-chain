#pragma once

#include "kinematicChain.hpp"
#include "scene.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <string>

class LeftPanel
{
public:
	static constexpr int width = 272;

	LeftPanel(Scene& scene, const glm::ivec2& viewportSize);
	void update();

private:
	Scene& m_scene;
	const glm::ivec2& m_viewportSize;

	void updateMode();
	void updateKinematicChainLengths();
	void updateKinematicChainPos(const std::function<glm::vec2(void)>& getter,
		const std::function<void(const glm::vec2&)>& setter, const std::string& suffix,
		bool disabled = false);
	void updateObstacle();
	void updateAnimationTime();
	void updateAnimationButtons();
	void updateTime();
};

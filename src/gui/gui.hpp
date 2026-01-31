#pragma once

#include "gui/configurationSpacePanel.hpp"
#include "gui/leftPanel.hpp"
#include "scene.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

class GUI
{
public:
	GUI(GLFWwindow* window, Scene& scene, const glm::ivec2& viewportSize);
	~GUI();

	void update();
	void render();

private:
	LeftPanel m_leftPanel;
	ConfigurationSpacePanel m_configurationSpacePanel;
};

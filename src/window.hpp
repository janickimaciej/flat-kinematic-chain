#pragma once

#include "gui/leftPanel.hpp"
#include "scene.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
	Window();
	~Window();

	void init(Scene& scene);
	bool shouldClose() const;
	void swapBuffers() const;
	void pollEvents() const;

	const glm::ivec2& viewportSize() const;
	GLFWwindow* getPtr();

private:
	static constexpr glm::ivec2 m_initialSize{1900, 1000};

	GLFWwindow* m_windowPtr{};
	glm::ivec2 m_viewportSize{m_initialSize - glm::ivec2{LeftPanel::width, 0}};
	Scene* m_scene{};

	glm::vec2 m_lastCursorPos{};
	bool m_dragging = false;

	void resizeCallback(int width, int height);
	void cursorMovementCallback(double x, double y);
	void buttonCallback(int button, int action, int);
	void keyCallback(int key, int, int action, int);

	void updateViewport() const;
	glm::vec2 getCursorPos() const;
	bool isButtonPressed(int button);
	bool isKeyPressed(int key);
	bool isCursorInGUI();

	template <auto callback, typename... Args>
	static void callbackWrapper(GLFWwindow* windowPtr, Args... args);
};

template <auto callback, typename... Args>
void Window::callbackWrapper(GLFWwindow* windowPtr, Args... args)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowPtr));
	(window->*callback)(args...);
}

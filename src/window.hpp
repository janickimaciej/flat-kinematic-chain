#pragma once

#include "guis/gui.hpp"
#include "scene.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
	Window(const glm::ivec2& initialSize);
	~Window();

	const glm::ivec2& viewportSize() const;
	void setWindowData(Scene& scene, GUI& gui);
	bool shouldClose() const;
	void swapBuffers() const;
	void pollEvents() const;
	GLFWwindow* getPtr();

private:
	GLFWwindow* m_windowPtr{};
	glm::ivec2 m_viewportSize{};
	Scene* m_scene{};
	GUI* m_gui{};

	glm::vec2 m_lastCursorPos{};
	bool m_dragging = false;

	void resizeCallback(int width, int height);
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

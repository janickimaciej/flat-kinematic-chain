#include "window.hpp"

#include "guis/leftPanel.hpp"
#include "shaderPrograms.hpp"

#include <cmath>
#include <string>

Window::Window(const glm::ivec2& initialSize) :
	m_viewportSize{initialSize - glm::ivec2{LeftPanel::width, 0}}
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	static const std::string windowTitle = "flat-kinematic-chain";
	m_windowPtr = glfwCreateWindow(initialSize.x, initialSize.y, windowTitle.c_str(), nullptr,
		nullptr);
	glfwSetWindowUserPointer(m_windowPtr, this);
	glfwMakeContextCurrent(m_windowPtr);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(m_windowPtr, callbackWrapper<&Window::resizeCallback>);
	//glfwSetCursorPosCallback(m_windowPtr, callbackWrapper<&Window::cursorMovementCallback>);
	glfwSetMouseButtonCallback(m_windowPtr, callbackWrapper<&Window::buttonCallback>);
	glfwSetKeyCallback(m_windowPtr, callbackWrapper<&Window::keyCallback>);

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	updateViewport();
	ShaderPrograms::init();
}

Window::~Window()
{
	glfwTerminate();
}

const glm::ivec2& Window::viewportSize() const
{
	return m_viewportSize;
}

void Window::setWindowData(Scene& scene, GUI& gui)
{
	m_scene = &scene;
	m_gui = &gui;
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(m_windowPtr);
}

void Window::swapBuffers() const
{
	glfwSwapBuffers(m_windowPtr);
}

void Window::pollEvents() const
{
	glfwPollEvents();
}

GLFWwindow* Window::getPtr()
{
	return m_windowPtr;
}

void Window::resizeCallback(int width, int height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	m_viewportSize = {width - LeftPanel::width, height};
	updateViewport();
}

void Window::cursorMovementCallback(double x, double y)
{
	glm::vec2 currPos{static_cast<float>(x), static_cast<float>(y)};

	if (isCursorInGUI())
	{
		return;
	}

	if (m_dragging)
	{
		m_scene->setChainScreenPos(currPos);
	}
}

void Window::buttonCallback(int button, int action, int)
{
	glm::vec2 cursorPos = getCursorPos();

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		m_dragging = false;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (isCursorInGUI())
		{
			return;
		}

		if (isKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_scene->selectObstacle(cursorPos);
		}
		else
		{
			m_scene->setChainScreenPos(cursorPos);
			m_dragging = true;
		}
	}
}

void Window::keyCallback(int key, int, int action, int)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		m_scene->cancel();
	}
}

void Window::updateViewport() const
{
	glViewport(LeftPanel::width, 0, m_viewportSize.x, m_viewportSize.y);
}

glm::vec2 Window::getCursorPos() const
{
	double x{};
	double y{};
	glfwGetCursorPos(m_windowPtr, &x, &y);
	return {static_cast<float>(x), static_cast<float>(y)};
}

bool Window::isButtonPressed(int button)
{
	return glfwGetMouseButton(m_windowPtr, button) == GLFW_PRESS;
}

bool Window::isKeyPressed(int key)
{
	return glfwGetKey(m_windowPtr, key) == GLFW_PRESS;
}

bool Window::isCursorInGUI()
{
	glm::vec2 cursorPos = getCursorPos();
	return cursorPos.x <= LeftPanel::width;
}

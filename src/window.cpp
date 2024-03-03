#include "window.hpp"

#include "scene.hpp"
#include "window_user_data.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>

Window::Window(int initialWidth, int initialHeight, Scene& scene, Texture& texture)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_userData.scene = &scene;
	m_userData.texture = &texture;
	const std::string windowTitle = "ellipsoid-raycasting";
	m_windowPtr = glfwCreateWindow(initialWidth, initialHeight, windowTitle.c_str(),
		nullptr, nullptr);
	glfwSetWindowUserPointer(m_windowPtr, &m_userData);
	glfwMakeContextCurrent(m_windowPtr);
	glfwSetFramebufferSizeCallback(m_windowPtr, resizeCallback);
	glfwSetCursorPosCallback(m_windowPtr, cursorMovementCallback);
	glfwSetScrollCallback(m_windowPtr, scrollCallback);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_windowPtr);
}

void Window::pollEvents()
{
	glfwPollEvents();
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_windowPtr);
}

GLFWwindow* Window::getPtr()
{
	return m_windowPtr;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	WindowUserData* userData = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));
	userData->scene->rescale(width, height);
	userData->texture->rescale(width, height);
	glViewport(0, 0, width, height);
}

void Window::cursorMovementCallback(GLFWwindow* window, double x, double y)
{
	static float previousX{};
	static float previousY{};

	float xOffset = x - previousX;
	float yOffset = y - previousY;
	previousX = x;
	previousY = y;
	
	WindowUserData* windowUserData =
		static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

	if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS &&
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) ||
		(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE &&
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS))
	{
		float sensitivity = 0.002f;
		windowUserData->scene->addAzimuth(-sensitivity * xOffset);
		windowUserData->scene->addElevation(sensitivity * yOffset);
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) &&
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		float sensitivity = 0.001f;
		windowUserData->scene->moveX(-sensitivity * xOffset);
		windowUserData->scene->moveY(sensitivity * yOffset);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS &&
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		float sensitivity = 1.005f;
		windowUserData->scene->zoom(std::pow(sensitivity, yOffset));
	}
}

void Window::scrollCallback(GLFWwindow* window, double, double yOffset)
{
	WindowUserData* windowUserData =
		static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

	float sensitivity = 1.1f;
	windowUserData->scene->zoom(std::pow(sensitivity, -yOffset));
}

Window::~Window()
{
	glfwTerminate();
}

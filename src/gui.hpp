#pragma once

#include "scene.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

class GUI
{
public:
	GUI(GLFWwindow* window, Scene* scene);
	void update();
	void render();
	~GUI();

private:
	Scene* m_scene{};
	int m_maxPixelSizeExponent{};
	float m_viewWidth{};
	float m_ambient{};
	float m_diffuse{};
	float m_specular{};
	float m_shininess{};
	float m_a{};
	float m_b{};
	float m_c{};

	void getValuesFromScene();
	void setValuesInScene();
};

#include "gui.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <algorithm>

GUI::GUI(GLFWwindow* window, Scene* scene) :
	m_scene{scene}
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

void GUI::update()
{
	getValuesFromScene();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
	ImGui::SetNextWindowSize({200, 1000}, ImGuiCond_Always);
	ImGui::Begin("Config", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoBackground);
	ImGui::PushItemWidth(100);

	ImGui::InputInt("accuracy", &m_maxPixelSizeExponent, 1, 1);
	m_maxPixelSizeExponent = std::max(m_maxPixelSizeExponent, 0);

	ImGui::InputFloat("view width", &m_viewWidth, 0.1f, 0.1f, "%.2f");
	m_viewWidth = std::max(m_viewWidth, 0.01f);

	ImGui::InputFloat("ambient", &m_ambient, 0.01f, 0.01f, "%.2f");
	m_ambient = std::clamp(m_ambient, 0.0f, 1.0f);

	ImGui::InputFloat("diffuse", &m_diffuse, 0.01f, 0.01f, "%.2f");
	m_diffuse = std::clamp(m_diffuse, 0.0f, 1.0f);

	ImGui::InputFloat("specular", &m_specular, 0.01f, 0.01f, "%.2f");
	m_specular = std::clamp(m_specular, 0.0f, 1.0f);

	ImGui::InputFloat("shininess", &m_shininess, 0.1f, 0.1f, "%.1f");
	m_shininess = std::clamp(m_shininess, 1.0f, 100.0f);

	ImGui::InputFloat("a", &m_a, 0.1f, 0.1f, "%.1f");
	m_a = std::max(m_a, 0.1f);

	ImGui::InputFloat("b", &m_b, 0.1f, 0.1f, "%.1f");
	m_b = std::max(m_b, 0.1f);

	ImGui::InputFloat("c", &m_c, 0.1f, 0.1f, "%.1f");
	m_c = std::max(m_c, 0.1f);

	ImGui::End();

	setValuesInScene();
}

void GUI::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::getValuesFromScene()
{
	m_maxPixelSizeExponent = m_scene->getMaxPixelSizeExponent();
	m_viewWidth = m_scene->getViewWidth();
	m_ambient = m_scene->getAmbient();
	m_diffuse = m_scene->getDiffuse();
	m_specular = m_scene->getSpecular();
	m_shininess = m_scene->getShininess();
	m_a = m_scene->getA();
	m_b = m_scene->getB();
	m_c = m_scene->getC();
}

void GUI::setValuesInScene()
{
	m_scene->setMaxPixelSizeExponent(m_maxPixelSizeExponent);
	m_scene->setViewWidth(m_viewWidth);
	m_scene->setAmbient(m_ambient);
	m_scene->setDiffuse(m_diffuse);
	m_scene->setSpecular(m_specular);
	m_scene->setShininess(m_shininess);
	m_scene->setA(m_a);
	m_scene->setB(m_b);
	m_scene->setC(m_c);
}

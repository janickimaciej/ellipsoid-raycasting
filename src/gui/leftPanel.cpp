#include "gui/leftPanel.hpp"

#include <imgui/imgui.h>

#include <algorithm>

LeftPanel::LeftPanel(Scene& scene, const glm::ivec2& viewportSize) :
	m_scene{scene},
	m_viewportSize{viewportSize}
{ }

void LeftPanel::update()
{
	ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
	ImGui::SetNextWindowSize({width, static_cast<float>(m_viewportSize.y)}, ImGuiCond_Always);
	ImGui::Begin("leftPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::PushItemWidth(100);

	updateIntValue("accuracy",
		[this] () { return m_scene.getAccuracy(); },
		[this] (int value) { m_scene.setAccuracy(value); },
		1, 0);
	updateFloatValue("view width",
		[this] () { return m_scene.getViewWidth(); },
		[this] (float value) { m_scene.setViewWidth(value); },
		0.1f, "%.2f", 0.01f);
	updateFloatValue("ambient",
		[this] () { return m_scene.getAmbient(); },
		[this] (float value) { m_scene.setAmbient(value); },
		0.01f, "%.2f", 0.0f, 1.0f);
	updateFloatValue("diffuse",
		[this] () { return m_scene.getDiffuse(); },
		[this] (float value) { m_scene.setDiffuse(value); },
		0.01f, "%.2f", 0.0f, 1.0f);
	updateFloatValue("specular",
		[this] () { return m_scene.getSpecular(); },
		[this] (float value) { m_scene.setSpecular(value); },
		0.01f, "%.2f", 0.0f, 1.0f);
	updateFloatValue("shininess",
		[this] () { return m_scene.getShininess(); },
		[this] (float value) { m_scene.setShininess(value); },
		0.1f, "%.1f", 1.0f, 100.0f);
	updateFloatValue("a",
		[this] () { return m_scene.getEllipsoidA(); },
		[this] (float value) { m_scene.setEllipsoidA(value); },
		0.1f, "%.1f", 0.1f);
	updateFloatValue("b",
		[this] () { return m_scene.getEllipsoidB(); },
		[this] (float value) { m_scene.setEllipsoidB(value); },
		0.1f, "%.1f", 0.1f);
	updateFloatValue("c",
		[this] () { return m_scene.getEllipsoidC(); },
		[this] (float value) { m_scene.setEllipsoidC(value); },
		0.1f, "%.1f", 0.1f);

	ImGui::PopItemWidth();
	ImGui::End();
}

void LeftPanel::updateIntValue(const std::string& name, const std::function<int(void)>& getter,
	const std::function<void(int)>& setter, int step, int min, int max)
{
	int value = getter();
	int prevValue = value;

	ImGui::InputInt(name.c_str(), &value, step, step);

	value = std::clamp(value, min, max);

	if (value != prevValue)
	{
		setter(value);
	}
}

void LeftPanel::updateFloatValue(const std::string& name, const std::function<float(void)>& getter,
	const std::function<void(float)>& setter, float step, const std::string& format, float min,
	float max)
{
	float value = getter();
	float prevValue = value;

	ImGui::InputFloat(name.c_str(), &value, step, step, format.c_str());

	value = std::clamp(value, min, max);

	if (value != prevValue)
	{
		setter(value);
	}
}

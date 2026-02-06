#pragma once

#include "scene.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <limits>
#include <string>

class LeftPanel
{
public:
	static constexpr int width = 200;

	LeftPanel(Scene& scene, const glm::ivec2& viewportSize);
	void update();

private:
	Scene& m_scene;
	const glm::ivec2& m_viewportSize;

	void updateIntValue(const std::string& name, const std::function<int(void)>& getter,
		const std::function<void(int)>& setter, int step, int min,
		int max = std::numeric_limits<int>::max());
	void updateFloatValue(const std::string& name, const std::function<float(void)>& getter,
		const std::function<void(float)>& setter, float step, const std::string& format, float min,
		float max = std::numeric_limits<float>::max());
};

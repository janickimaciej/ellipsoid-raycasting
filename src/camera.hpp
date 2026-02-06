#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::ivec2& viewportSize, float nearPlane, float farPlane, float viewWidth);

	glm::mat4 getMatrixInverse() const;
	void updateViewportSize();
	float getViewWidth() const;
	void setViewWidth(float viewWidth);

	glm::vec3 getPos() const;
	void moveX(float x);
	void moveY(float y);
	void addPitch(float pitchRad);
	void addYaw(float yawRad);
	void zoom(float zoom);

private:
	const glm::ivec2& m_viewportSize{};
	float m_nearPlane{};
	float m_farPlane{};
	float m_viewWidth{};

	glm::vec3 m_targetPos{0, 0, 0};
	float m_pitchRad = 0;
	float m_yawRad = 0;
	float m_radius = 500;

	glm::mat4 m_viewMatrixInverse{1};
	glm::mat4 m_projectionMatrix{1};

	void updateViewMatrix();
	void updateProjectionMatrix();
};

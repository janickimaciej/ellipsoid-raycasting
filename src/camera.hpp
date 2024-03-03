#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(float width, float aspectRatio, float nearPlane, float farPlane);
	glm::mat4 getProjectionViewMatrixInverse() const;
	glm::vec3 getPosition() const;
	void addElevation(float elevationRad);
	void addAzimuth(float azimuthRad);
	void addRadius(float radius);
	void moveX(float x);
	void moveY(float y);
	void zoom(float zoom);
	void setAspectRatio(float aspectRatio);
	float getViewWidth() const;
	void setViewWidth(float viewWidth);

private:
	float m_viewWidth{};
	float m_aspectRatio{};
	float m_nearPlane{};
	float m_farPlane{};
	glm::vec3 m_targetPosition{0, 0, 0};
	float m_radius = 500;
	float m_elevationRad = 0;
	float m_azimuthRad = 0;
	glm::mat4 m_viewMatrixInverse{1};
	glm::mat4 m_projectionMatrix{1};
	glm::mat4 m_projectionViewMatrixInverse{1};

	void updateViewMatrixInverse();
	void updateProjectionMatrix();
	void updateProjectionViewMatrixInverse();
};

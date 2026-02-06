#include "camera.hpp"

#include <glm/gtc/constants.hpp>

#include <cmath>

Camera::Camera(const glm::ivec2& viewportSize, float nearPlane, float farPlane, float viewWidth) :
	m_viewportSize{viewportSize},
	m_nearPlane{nearPlane},
	m_farPlane{farPlane},
	m_viewWidth{viewWidth}
{
	updateViewMatrix();
	updateProjectionMatrix();
}

glm::mat4 Camera::getMatrixInverse() const
{
	return m_viewMatrixInverse * glm::inverse(m_projectionMatrix);
}

void Camera::updateViewportSize()
{
	updateProjectionMatrix();
}

float Camera::getViewWidth() const
{
	return m_viewWidth;
}

void Camera::setViewWidth(float viewWidth)
{
	m_viewWidth = viewWidth;

	updateProjectionMatrix();
}

glm::vec3 Camera::getPos() const
{
	return m_targetPos + m_radius *
		glm::vec3
		{
			-std::cos(m_pitchRad) * std::sin(m_yawRad),
			-std::sin(m_pitchRad),
			std::cos(m_pitchRad) * std::cos(m_yawRad)
		};
}

void Camera::moveX(float x)
{
	m_targetPos += m_viewWidth * glm::mat3{m_viewMatrixInverse} * glm::vec3{x, 0, 0};

	updateViewMatrix();
}

void Camera::moveY(float y)
{
	m_targetPos += m_viewWidth * glm::mat3{m_viewMatrixInverse} * glm::vec3{0, y, 0};

	updateViewMatrix();
}

void Camera::addPitch(float pitchRad)
{
	m_pitchRad += pitchRad;

	constexpr float bound = glm::radians(89.0f);
	if (m_pitchRad < -bound)
	{
		m_pitchRad = -bound;
	}
	if (m_pitchRad > bound)
	{
		m_pitchRad = bound;
	}

	updateViewMatrix();
}

void Camera::addYaw(float yawRad)
{
	m_yawRad += yawRad;

	constexpr float pi = glm::pi<float>();
	while (m_yawRad < -pi)
	{
		m_yawRad += 2 * pi;
	}
	while (m_yawRad >= pi)
	{
		m_yawRad -= 2 * pi;
	}

	updateViewMatrix();
}

void Camera::zoom(float zoom)
{
	m_viewWidth /= zoom;

	updateProjectionMatrix();
}

void Camera::updateViewMatrix()
{
	glm::vec3 pos = getPos();

	glm::vec3 direction = glm::normalize(pos - m_targetPos);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3{0, 1, 0}, direction));
	glm::vec3 up = glm::cross(direction, right);

	m_viewMatrixInverse =
		{
			right.x, right.y, right.z, 0,
			up.x, up.y, up.z, 0,
			direction.x, direction.y, direction.z, 0,
			pos.x, pos.y, pos.z, 1
		};
}

void Camera::updateProjectionMatrix()
{
	float aspectRatio = static_cast<float>(m_viewportSize.x) / m_viewportSize.y;
	float viewHeight = m_viewWidth / aspectRatio;

	m_projectionMatrix =
		glm::mat4
		{
			2 / m_viewWidth, 0, 0, 0,
			0, 2 / viewHeight, 0, 0,
			0, 0, -2 / (m_farPlane - m_nearPlane), 0,
			0, 0, -(m_farPlane + m_nearPlane) / (m_farPlane - m_nearPlane), 1
		};
}

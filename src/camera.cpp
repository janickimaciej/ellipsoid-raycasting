#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

Camera::Camera(float viewWidth, float aspectRatio, float nearPlane, float farPlane) :
	m_viewWidth{viewWidth},
	m_aspectRatio{aspectRatio},
	m_nearPlane{nearPlane},
	m_farPlane{farPlane}
{
	updateViewMatrixInverse();

	setAspectRatio(aspectRatio);
}

glm::mat4 Camera::getProjectionViewMatrixInverse() const
{
	return m_projectionViewMatrixInverse;
}

glm::vec3 Camera::getPosition() const
{
	return glm::vec3{m_viewMatrixInverse[3][0], m_viewMatrixInverse[3][1],
		m_viewMatrixInverse[3][2]};
}

void Camera::addElevation(float elevationRad)
{
	m_elevationRad += elevationRad;

	constexpr float bound = glm::radians(89.0f);
	if (m_elevationRad < -bound)
	{
		m_elevationRad = -bound;
	}
	if (m_elevationRad > bound)
	{
		m_elevationRad = bound;
	}
	
	updateViewMatrixInverse();
	updateProjectionViewMatrixInverse();
}

void Camera::addAzimuth(float azimuthRad)
{
	m_azimuthRad += azimuthRad;
	
	constexpr float pi = glm::pi<float>();
	while (m_azimuthRad < -pi)
	{
		m_azimuthRad += 2 * pi;
	}
	while (m_azimuthRad >= pi)
	{
		m_azimuthRad -= 2 * pi;
	}

	updateViewMatrixInverse();
	updateProjectionViewMatrixInverse();
}

void Camera::addRadius(float radius)
{
	m_radius += radius;

	if (m_radius < 0.1f)
	{
		m_radius = 0.1f;
	}
	
	updateViewMatrixInverse();
	updateProjectionViewMatrixInverse();
}

void Camera::moveX(float x)
{
	m_targetPosition += m_viewWidth * glm::mat3{m_viewMatrixInverse} * glm::vec3{x, 0, 0};

	updateViewMatrixInverse();
	updateProjectionViewMatrixInverse();
}

void Camera::moveY(float y)
{
	m_targetPosition += m_viewWidth * glm::mat3{m_viewMatrixInverse} * glm::vec3{0, y, 0};

	updateViewMatrixInverse();
	updateProjectionViewMatrixInverse();
}

void Camera::zoom(float zoom)
{
	m_viewWidth *= zoom;

	updateProjectionMatrix();
	updateProjectionViewMatrixInverse();
}

void Camera::setAspectRatio(float aspectRatio)
{
	m_aspectRatio = aspectRatio;

	updateProjectionMatrix();
	updateProjectionViewMatrixInverse();
}

float Camera::getViewWidth() const
{
	return m_viewWidth;
}

void Camera::setViewWidth(float viewWidth)
{
	m_viewWidth = viewWidth;

	updateProjectionMatrix();
	updateProjectionViewMatrixInverse();
}

void Camera::updateViewMatrixInverse()
{
	glm::vec3 position = m_targetPosition + m_radius *
		glm::vec3
		{
			std::cos(m_elevationRad) * std::sin(m_azimuthRad),
			std::sin(m_elevationRad),
			std::cos(m_elevationRad) * std::cos(m_azimuthRad)
		};

	glm::vec3 direction = glm::normalize(position - m_targetPosition);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3{0, 1, 0}, direction));
	glm::vec3 up = glm::cross(direction, right);

	m_viewMatrixInverse =
	{
		right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		direction.x, direction.y, direction.z, 0,
		position.x, position.y, position.z, 1
	};
}

void Camera::updateProjectionMatrix()
{
	float viewHeight = m_viewWidth / m_aspectRatio;
	m_projectionMatrix =
		glm::mat4
		{
			2 / m_viewWidth, 0, 0, 0,
			0, 2 / viewHeight, 0, 0,
			0, 0, -2 / (m_farPlane - m_nearPlane), 0,
			0, 0, -(m_farPlane + m_nearPlane) / (m_farPlane - m_nearPlane), 1
		};
}

void Camera::updateProjectionViewMatrixInverse()
{
	m_projectionViewMatrixInverse = m_viewMatrixInverse * glm::inverse(m_projectionMatrix);
}

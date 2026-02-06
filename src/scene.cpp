#include "scene.hpp"

#include "material.hpp"
#include "shaderPrograms.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <cmath>
#include <cstddef>

constexpr float nearPlane = 0.0f;
constexpr float farPlane = 1000.0f;
constexpr float initViewWidth = 20.0f;

Scene::Scene(const glm::ivec2& viewportSize) :
	m_viewportSize{viewportSize},
	m_camera{viewportSize, nearPlane, farPlane, initViewWidth},
	m_texture{viewportSize},
	m_cpuTexture(m_numOfChannels * viewportSize.x * viewportSize.y, 0)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
}

void Scene::render()
{
	static constexpr glm::vec3 backgroundColor{0.1f, 0.1f, 0.1f};
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_pixelSize > 0)
	{
		draw();
		m_texture.overwrite(m_cpuTexture);
		m_pixelSize /= 2;
	}

	m_texture.use();
	ShaderPrograms::quad->use();
	m_quad.render();
}

void Scene::updateViewportSize()
{
	m_camera.updateViewportSize();
	m_texture.rescale(m_viewportSize);
	m_cpuTexture =
		std::vector<unsigned char>(m_numOfChannels * m_viewportSize.x * m_viewportSize.y, 0);
	refresh();
}

void Scene::moveXCamera(float x)
{
	m_camera.moveX(x);
	refresh();
}

void Scene::moveYCamera(float y)
{
	m_camera.moveY(y);
	refresh();
}

void Scene::addPitchCamera(float pitchRad)
{
	m_camera.addPitch(pitchRad);
	refresh();
}

void Scene::addYawCamera(float yawRad)
{
	m_camera.addYaw(yawRad);
	refresh();
}

void Scene::zoomCamera(float zoom)
{
	m_camera.zoom(zoom);
	refresh();
}

int Scene::getAccuracy() const
{
	return m_maxPixelSizeExponent;
}

void Scene::setAccuracy(int maxPixelSizeExponent)
{
	m_maxPixelSizeExponent = maxPixelSizeExponent;
	refresh();
}

float Scene::getViewWidth() const
{
	return m_camera.getViewWidth();
}

void Scene::setViewWidth(float viewWidth)
{
	m_camera.setViewWidth(viewWidth);
	refresh();
}

float Scene::getAmbient() const
{
	return m_ellipsoid.getMaterial().ambientCoef;
}

void Scene::setAmbient(float ambient)
{
	Material material = m_ellipsoid.getMaterial();
	material.ambientCoef = ambient;
	m_ellipsoid.setMaterial(material);
	refresh();
}

float Scene::getDiffuse() const
{
	return m_ellipsoid.getMaterial().diffuseCoef;
}

void Scene::setDiffuse(float diffuse)
{
	Material material = m_ellipsoid.getMaterial();
	material.diffuseCoef = diffuse;
	m_ellipsoid.setMaterial(material);
	refresh();
}

float Scene::getSpecular() const
{
	return m_ellipsoid.getMaterial().specularCoef;
}

void Scene::setSpecular(float specular)
{
	Material material = m_ellipsoid.getMaterial();
	material.specularCoef = specular;
	m_ellipsoid.setMaterial(material);
	refresh();
}

float Scene::getShininess() const
{
	return m_ellipsoid.getMaterial().shininess;
}

void Scene::setShininess(float shininess)
{
	Material material = m_ellipsoid.getMaterial();
	material.shininess = shininess;
	m_ellipsoid.setMaterial(material);
	refresh();
}

float Scene::getEllipsoidA() const
{
	return m_ellipsoid.getA();
}

void Scene::setEllipsoidA(float a)
{
	m_ellipsoid.setA(a);
	refresh();
}

float Scene::getEllipsoidB() const
{
	return m_ellipsoid.getB();
}

void Scene::setEllipsoidB(float b)
{
	m_ellipsoid.setB(b);
	refresh();
}

float Scene::getEllipsoidC() const
{
	return m_ellipsoid.getC();
}

void Scene::setEllipsoidC(float c)
{
	m_ellipsoid.setC(c);
	refresh();
}

void Scene::refresh()
{
	m_pixelSize = getMaxPixelSize();
}

void Scene::draw()
{
	glm::vec3 cameraPos = m_camera.getPos();
	glm::mat4 cameraMatrix = m_camera.getMatrixInverse();
	glm::mat4 cameraEllipsoidMatrix =
		glm::transpose(cameraMatrix) * m_ellipsoid.getMatrix() * cameraMatrix;

	const int halfPixelSize = m_pixelSize / 2;
	bool isRowIndexEven = true;
	for (int centerY = 0; centerY < m_viewportSize.y + halfPixelSize;
		centerY += m_pixelSize, isRowIndexEven = !isRowIndexEven)
	{
		int start{};
		int increment{};
		if (m_pixelSize != getMaxPixelSize() && isRowIndexEven)
		{
			start = m_pixelSize;
			increment = 2 * m_pixelSize;
		}
		else
		{
			start = 0;
			increment = m_pixelSize;
		}

		for (int centerX = start; centerX < m_viewportSize.x + halfPixelSize; centerX += increment)
		{
			glm::ivec3 color = calcColor(cameraPos, cameraMatrix, cameraEllipsoidMatrix,
				2 * static_cast<float>(centerX) / m_viewportSize.x - 1,
				2 * static_cast<float>(centerY) / m_viewportSize.y - 1);

			int startY = std::max(centerY - halfPixelSize, 0);
			int endY =
				std::min(centerY + (halfPixelSize != 0 ? halfPixelSize : 1), m_viewportSize.y);
			for (std::size_t y = startY; y < endY; ++y)
			{
				int startX = std::max(centerX - halfPixelSize, 0);
				int endX =
					std::min(centerX + (halfPixelSize != 0 ? halfPixelSize : 1), m_viewportSize.x);
				for (std::size_t x = startX; x < endX; ++x)
				{
					for (int channel = 0; channel < m_numOfChannels; ++channel)
					{
						m_cpuTexture[(y * m_viewportSize.x + x) * m_numOfChannels + channel] =
							static_cast<unsigned char>(color[channel]);
					}
				}
			}
		}
	}
}

glm::ivec3 Scene::calcColor(const glm::vec3& cameraPos, const glm::mat4& cameraMatrix,
	const glm::mat4& cameraEllipsoidMatrix, float x, float y) const
{
	std::optional<float> z = calcIntersection(x, y, cameraEllipsoidMatrix);
	constexpr glm::ivec3 background{30, 30, 30};
	if (!z.has_value())
	{
		return background;
	}

	return calcPhong(glm::vec3{cameraMatrix * glm::vec4{x, y, *z, 1}}, cameraPos);
}

std::optional<float> Scene::calcIntersection(float x, float y,
	const glm::mat4& cameraEllipsoidMatrix) const
{
	float a = cameraEllipsoidMatrix[2][2];
	float b = (cameraEllipsoidMatrix[2][0] + cameraEllipsoidMatrix[0][2]) * x +
		(cameraEllipsoidMatrix[2][1] + cameraEllipsoidMatrix[1][2]) * y +
		cameraEllipsoidMatrix[2][3] + cameraEllipsoidMatrix[3][2];
	float c = (cameraEllipsoidMatrix[0][0] * x + cameraEllipsoidMatrix[0][1] * y +
		cameraEllipsoidMatrix[0][3] + cameraEllipsoidMatrix[3][0]) * x +
		(cameraEllipsoidMatrix[1][0] * x + cameraEllipsoidMatrix[1][1] * y +
		cameraEllipsoidMatrix[1][3] + cameraEllipsoidMatrix[3][1]) * y +
		cameraEllipsoidMatrix[3][3];

	float delta = b * b - 4 * a * c;

	if (delta <= 0)
	{
		return std::nullopt;
	}

	float z = (-b - std::sqrt(delta)) / (2 * a);

	if (z < -1 || z > 1)
	{
		return std::nullopt;
	}

	return z;
}

glm::ivec3 Scene::calcPhong(const glm::vec3& point, const glm::vec3& cameraPos) const
{
	Material material = m_ellipsoid.getMaterial();

	glm::vec3 normalVector = m_ellipsoid.getNormalVector(point);
	glm::vec3 viewVector = glm::normalize(cameraPos);
	glm::vec3 lightVector = viewVector;

	float ambient = material.ambientCoef;

	float lightNormalCos = glm::dot(lightVector, normalVector);
	float diffuse = lightNormalCos > 0 ? material.diffuseCoef * lightNormalCos : 0;

	glm::vec3 reflectionVector =
		2 * glm::dot(lightVector, normalVector) * normalVector - lightVector;
	float reflectionViewCos = glm::dot(reflectionVector, viewVector);
	float specular = reflectionViewCos > 0 ?
		material.specularCoef * std::pow(reflectionViewCos, material.shininess) :
		0;

	glm::ivec3 color = (ambient + diffuse + specular) * glm::vec3{material.color};
	color.r = std::clamp(color.r, 0, 255);
	color.g = std::clamp(color.g, 0, 255);
	color.b = std::clamp(color.b, 0, 255);
	return color;
}

int Scene::getMaxPixelSize() const
{
	return 1 << m_maxPixelSizeExponent;
}

#include "scene.hpp"

#include "material.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <cmath>

constexpr float viewWidth = 20.0f;
constexpr float nearPlane = 0.0f;
constexpr float farPlane = 1000.0f;

Scene::Scene(int initialWidth, int initialHeight) :
	m_pixelSize{getMaxPixelSize()},
	m_width{initialWidth},
	m_height{initialHeight},
	m_cpuTexture(m_numOfChannels * initialWidth * initialHeight, 0),
	m_camera{viewWidth, static_cast<float>(initialWidth) / initialHeight, nearPlane, farPlane}
{ }

bool Scene::renderToTexture(Texture& texture)
{
	if (m_pixelSize > 0)
	{
		draw();
		texture.overwrite(m_cpuTexture);
		m_pixelSize /= 2;
		return true;
	}
	return false;
}

void Scene::addElevation(float elevationRad)
{
	m_camera.addElevation(elevationRad);
	refresh();
}

void Scene::addAzimuth(float azimuthRad)
{
	m_camera.addAzimuth(azimuthRad);
	refresh();
}

void Scene::addRadius(float radius)
{
	m_camera.addRadius(radius);
	refresh();
}

void Scene::moveX(float x)
{
	m_camera.moveX(x);
	refresh();
}

void Scene::moveY(float y)
{
	m_camera.moveY(y);
	refresh();
}

void Scene::zoom(float zoom)
{
	m_camera.zoom(zoom);
	refresh();
}

void Scene::rescale(int width, int height)
{
	m_width = width;
	m_height = height;
	m_cpuTexture = std::vector<unsigned char>(m_numOfChannels * width * height, 0);
	m_camera.setAspectRatio(static_cast<float>(width) / height);
	refresh();
}

int Scene::getMaxPixelSizeExponent() const
{
	return m_maxPixelSizeExponent;
}

float Scene::getViewWidth() const
{
	return m_camera.getViewWidth();
}

float Scene::getAmbient() const
{
	return m_ellipsoid.getMaterial().ambientCoef;
}

float Scene::getDiffuse() const
{
	return m_ellipsoid.getMaterial().diffuseCoef;
}

float Scene::getSpecular() const
{
	return m_ellipsoid.getMaterial().specularCoef;
}

float Scene::getShininess() const
{
	return m_ellipsoid.getMaterial().shininess;
}

float Scene::getA() const
{
	return m_ellipsoid.getA();
}

float Scene::getB() const
{
	return m_ellipsoid.getB();
}

float Scene::getC() const
{
	return m_ellipsoid.getC();
}

void Scene::setMaxPixelSizeExponent(int maxPixelSizeExponent)
{
	if (maxPixelSizeExponent != m_maxPixelSizeExponent)
	{
		m_maxPixelSizeExponent = maxPixelSizeExponent;
		refresh();
	}
}

void Scene::setViewWidth(float viewWidth)
{
	if (viewWidth != m_camera.getViewWidth())
	{
		m_camera.setViewWidth(viewWidth);
		refresh();
	}
}

void Scene::setAmbient(float ambient)
{
	Material material = m_ellipsoid.getMaterial();
	if (ambient != material.ambientCoef)
	{
		material.ambientCoef = ambient;
		m_ellipsoid.setMaterial(material);
		refresh();
	}
}

void Scene::setDiffuse(float diffuse)
{
	Material material = m_ellipsoid.getMaterial();
	if (diffuse != material.diffuseCoef)
	{
		material.diffuseCoef = diffuse;
		m_ellipsoid.setMaterial(material);
		refresh();
	}
}

void Scene::setSpecular(float specular)
{
	Material material = m_ellipsoid.getMaterial();
	if (specular != material.specularCoef)
	{
		material.specularCoef = specular;
		m_ellipsoid.setMaterial(material);
		refresh();
	}
}

void Scene::setShininess(float shininess)
{
	Material material = m_ellipsoid.getMaterial();
	if (shininess != material.shininess)
	{
		material.shininess = shininess;
		m_ellipsoid.setMaterial(material);
		refresh();
	}
}

void Scene::setA(float a)
{
	if (a != m_ellipsoid.getA())
	{
		m_ellipsoid.setA(a);
		refresh();
	}
}

void Scene::setB(float b)
{
	if (b != m_ellipsoid.getB())
	{
		m_ellipsoid.setB(b);
		refresh();
	}
}

void Scene::setC(float c)
{
	if (c != m_ellipsoid.getC())
	{
		m_ellipsoid.setC(c);
		refresh();
	}
}

void Scene::refresh()
{
	m_pixelSize = getMaxPixelSize();
}

void Scene::draw()
{
	glm::mat4 cameraMatrix = m_camera.getProjectionViewMatrixInverse();
	glm::mat4 cameraEllipsoidMatrix =
		glm::transpose(cameraMatrix) * m_ellipsoid.getMatrix() * cameraMatrix;

	const int halfPixelSize = m_pixelSize / 2;
	bool isRowNumEven = true;
	for (int centerY = 0; centerY < m_height + halfPixelSize;
		centerY += m_pixelSize, isRowNumEven = !isRowNumEven)
	{
		int start{};
		int increment{};
		if (m_pixelSize != getMaxPixelSize() && isRowNumEven)
		{
			start = m_pixelSize;
			increment = 2 * m_pixelSize;
		}
		else
		{
			start = 0;
			increment = m_pixelSize;
		}

		for (int centerX = start; centerX < m_width + halfPixelSize; centerX += increment)
		{
			glm::ivec3 color = calcColor(cameraEllipsoidMatrix,
				2 * static_cast<float>(centerX) / m_width - 1,
				2 * static_cast<float>(centerY) / m_height - 1);

			int startY = std::max(centerY - halfPixelSize, 0);
			int endY = std::min(centerY + (halfPixelSize != 0 ? halfPixelSize : 1), m_height);
			for (int y = startY; y < endY; ++y)
			{
				int startX = std::max(centerX - halfPixelSize, 0);
				int endX = std::min(centerX + (halfPixelSize != 0 ? halfPixelSize : 1), m_width);
				for (int x = startX; x < endX; ++x)
				{
					for (int channel = 0; channel < m_numOfChannels; ++channel)
					{
						m_cpuTexture[(y * m_width + x) * m_numOfChannels + channel] =
							static_cast<unsigned char>(color[channel]);
					}
				}
			}
		}
	}
}

glm::ivec3 Scene::calcColor(const glm::mat4& cameraEllipsoidMatrix, float x, float y) const
{
	std::optional<float> z = calcIntersection(x, y, cameraEllipsoidMatrix);
	constexpr glm::ivec3 background{30, 30, 30};
	if (!z.has_value())
	{
		return background;
	}

	return calcPhong(glm::vec3{m_camera.getProjectionViewMatrixInverse() * glm::vec4{x, y, *z, 1}});
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

glm::ivec3 Scene::calcPhong(const glm::vec3& point) const
{
	Material material = m_ellipsoid.getMaterial();

	glm::vec3 normalVector = m_ellipsoid.getNormalVector(point);
	glm::vec3 viewVector = glm::normalize(m_camera.getPosition());
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

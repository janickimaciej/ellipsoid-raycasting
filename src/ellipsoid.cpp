#include "ellipsoid.hpp"

#include <glm/gtx/matrix_operation.hpp>

Ellipsoid::Ellipsoid(float a, float b, float c) :
	m_a{a},
	m_b{b},
	m_c{c},
	m_material
	{
		{255, 255, 0},
		0.1f,
		0.5f,
		0.9f,
		20
	}
{ }

glm::mat4 Ellipsoid::getMatrix() const
{
	return glm::diagonal4x4(
		glm::vec4
		{
			1.0f / (m_a * m_a),
			1.0f / (m_b * m_b),
			1.0f / (m_c * m_c),
			-1
		}
	);
}

glm::vec3 Ellipsoid::getNormalVector(const glm::vec3& point) const
{
	return glm::normalize(
		glm::vec3
		{
			1.0f / (m_a * m_a) * point.x,
			1.0f / (m_b * m_b) * point.y,
			1.0f / (m_c * m_c) * point.z
		}
	);
}

Material Ellipsoid::getMaterial() const
{
	return m_material;
}

float Ellipsoid::getA() const
{
	return m_a;
}

float Ellipsoid::getB() const
{
	return m_b;
}

float Ellipsoid::getC() const
{
	return m_c;
}

void Ellipsoid::setMaterial(const Material& material)
{
	m_material = material;
}

void Ellipsoid::setA(float a)
{
	m_a = a;
}

void Ellipsoid::setB(float b)
{
	m_b = b;
}

void Ellipsoid::setC(float c)
{
	m_c = c;
}

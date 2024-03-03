#pragma once

#include "material.hpp"

#include <glm/glm.hpp>

class Ellipsoid
{
public:
	Ellipsoid(float a, float b, float c);
	glm::mat4 getMatrix() const;
	glm::vec3 getNormalVector(const glm::vec3& point) const;
	Material getMaterial() const;
	float getA() const;
	float getB() const;
	float getC() const;
	void setMaterial(const Material& material);
	void setA(float a);
	void setB(float b);
	void setC(float c);

private:
	float m_a{};
	float m_b{};
	float m_c{};
	Material m_material;
};

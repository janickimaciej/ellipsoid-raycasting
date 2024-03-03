#pragma once

#include "canvas.hpp"
#include "camera.hpp"
#include "ellipsoid.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>

#include <optional>
#include <vector>

class Scene
{
public:
	Scene(int initialWidth, int initialHeight);
	bool renderToTexture(Texture& texture);
	void addElevation(float elevationRad);
	void addAzimuth(float azimuthRad);
	void addRadius(float radius);
	void moveX(float x);
	void moveY(float y);
	void zoom(float zoom);
	void rescale(int width, int height);

	int getMaxPixelSizeExponent() const;
	float getViewWidth() const;
	float getAmbient() const;
	float getDiffuse() const;
	float getSpecular() const;
	float getShininess() const;
	float getA() const;
	float getB() const;
	float getC() const;

	void setMaxPixelSizeExponent(int maxPixelSizeExponent);
	void setViewWidth(float viewWidth);
	void setAmbient(float ambient);
	void setDiffuse(float diffuse);
	void setSpecular(float specular);
	void setShininess(float shininess);
	void setA(float a);
	void setB(float b);
	void setC(float c);

private:
	static constexpr int m_numOfChannels = 3;
	int m_maxPixelSizeExponent = 4;
	int m_pixelSize{};
	int m_width{};
	int m_height{};
	std::vector<unsigned char> m_cpuTexture{};
	Ellipsoid m_ellipsoid{4.0f, 2.0f, 8.0f};
	Camera m_camera;

	void refresh();
	void draw();
	glm::ivec3 calcColor(const glm::mat4& cameraEllipsoidMatrix, float x, float y) const;
	std::optional<float> calcIntersection(float x, float y,
		const glm::mat4& cameraEllipsoidMatrix) const;
	glm::ivec3 calcPhong(const glm::vec3& point) const;
	int getMaxPixelSize() const;
};

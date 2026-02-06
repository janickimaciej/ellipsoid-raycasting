#pragma once

#include "camera.hpp"
#include "ellipsoid.hpp"
#include "quad.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>

#include <optional>
#include <vector>

class Scene
{
public:
	Scene(const glm::ivec2& viewportSize);

	void render();
	void updateViewportSize();

	void moveXCamera(float x);
	void moveYCamera(float y);
	void addPitchCamera(float pitchRad);
	void addYawCamera(float yawRad);
	void zoomCamera(float zoom);

	int getAccuracy() const;
	void setAccuracy(int maxPixelSizeExponent);
	float getViewWidth() const;
	void setViewWidth(float viewWidth);

	float getAmbient() const;
	void setAmbient(float ambient);
	float getDiffuse() const;
	void setDiffuse(float diffuse);
	float getSpecular() const;
	void setSpecular(float specular);
	float getShininess() const;
	void setShininess(float shininess);

	float getEllipsoidA() const;
	void setEllipsoidA(float a);
	float getEllipsoidB() const;
	void setEllipsoidB(float b);
	float getEllipsoidC() const;
	void setEllipsoidC(float c);

private:
	const glm::ivec2& m_viewportSize;
	Camera m_camera;
	Ellipsoid m_ellipsoid{4.0f, 2.0f, 8.0f};
	Quad m_quad{};
	Texture m_texture;

	static constexpr int m_numOfChannels = 3;
	int m_maxPixelSizeExponent = 4;
	int m_pixelSize = getMaxPixelSize();
	std::vector<unsigned char> m_cpuTexture{};

	void refresh();
	void draw();
	glm::ivec3 calcColor(const glm::vec3& cameraPos, const glm::mat4& cameraMatrix,
		const glm::mat4& cameraEllipsoidMatrix, float x, float y) const;
	std::optional<float> calcIntersection(float x, float y,
		const glm::mat4& cameraEllipsoidMatrix) const;
	glm::ivec3 calcPhong(const glm::vec3& point, const glm::vec3& cameraPos) const;
	int getMaxPixelSize() const;
};

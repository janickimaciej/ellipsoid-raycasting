#pragma once

#include <glm/glm.hpp>

#include <vector>

class Texture
{
public:
	Texture(const glm::ivec2& size);
	void use() const;
	void overwrite(const std::vector<unsigned char>& cpuTexture) const;
	void rescale(const glm::ivec2& size);
	~Texture();

private:
	unsigned int m_id{};
	glm::ivec2 m_size{};

	void create();
	void destroy() const;
};

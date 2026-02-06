#include "texture.hpp"

#include <glad/glad.h>

Texture::Texture(const glm::ivec2& size) :
	m_size{size}
{
	create();
}

void Texture::use() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::overwrite(const std::vector<unsigned char>& cpuTexture) const
{
	use();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size.x, m_size.y, GL_RGB, GL_UNSIGNED_BYTE,
		cpuTexture.data());
}

void Texture::rescale(const glm::ivec2& size)
{
	m_size = size;
	destroy();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	create();
}

Texture::~Texture()
{
	destroy();
}

void Texture::create()
{
	glGenTextures(1, &m_id);
	use();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_size.x, m_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE,
		nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::destroy() const
{
	glDeleteTextures(1, &m_id);
}

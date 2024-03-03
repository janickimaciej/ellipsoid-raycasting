#pragma once

#include <vector>

class Texture
{
public:
	Texture(int initialWidth, int initialHeight);
	void initialize();
	void use() const;
	void overwrite(const std::vector<unsigned char>& cpuTexture) const;
	void rescale(int width, int height);
	~Texture();

private:
	bool m_isInitialized = false;
	unsigned int m_id{};
	int m_width{};
	int m_height{};
	
	void create();
	void destroy() const;
};

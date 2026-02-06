#pragma once

class Quad
{
public:
	Quad();
	void render();

private:
	unsigned int m_VBO{};
	unsigned int m_VAO{};
};

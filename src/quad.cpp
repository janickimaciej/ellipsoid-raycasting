#include "quad.hpp"

#include <glad/glad.h>

#include <array>

constexpr int vertexCount = 6;
constexpr int vertexCoordinates = 3;

Quad::Quad()
{
	createVBO();
	createVAO();
}

Quad::~Quad()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void Quad::render() const
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount * vertexCoordinates));
	glBindVertexArray(0);
}

void Quad::createVBO()
{
	static constexpr std::array<float, vertexCount * vertexCoordinates> vertices
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
		vertices.data(), GL_STATIC_DRAW);
}

void Quad::createVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexCoordinates * sizeof(float),
		reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

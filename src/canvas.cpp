#include "canvas.hpp"

#include <glad/glad.h>

#include <array>

constexpr int vertexCount = 6;
constexpr int vertexCoordinates = 3;

Canvas::Canvas()
{
	std::array<float, vertexCount * vertexCoordinates> vertices =
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertices.size() * sizeof(float)), vertices.data(),
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexCoordinates * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void Canvas::render()
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(vertexCount * vertexCoordinates));
	glBindVertexArray(0);
}

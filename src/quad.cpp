#include "quad.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <array>

static constexpr int vertexCount = 6;

Quad::Quad()
{
	static constexpr std::array<glm::vec2, vertexCount> vertices
	{{
		{-1.0f, -1.0f},
		{1.0f, 1.0f},
		{-1.0f, 1.0f},
		{-1.0f, -1.0f},
		{1.0f, -1.0f},
		{1.0f, 1.0f}
	}};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec2)),
		vertices.data(), GL_STATIC_DRAW);
}

void Quad::render() const
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
	glBindVertexArray(0);
}

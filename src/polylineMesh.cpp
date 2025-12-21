#include "polylineMesh.hpp"

#include <glad/glad.h>

PolylineMesh::PolylineMesh(const std::vector<glm::vec2>& vertices)
{
	glGenVertexArrays(1, &m_VAO);
	createVBO(vertices);
}

PolylineMesh::~PolylineMesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void PolylineMesh::render(float width) const
{
	glBindVertexArray(m_VAO);
	glLineWidth(width);
	glDrawArrays(GL_LINE_STRIP, 0, m_vertexCount);
	glBindVertexArray(0);
}

void PolylineMesh::update(const std::vector<glm::vec2>& vertices)
{
	m_vertexCount = vertices.size();
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec2)),
		vertices.data(), GL_DYNAMIC_DRAW);
}

void PolylineMesh::createVBO(const std::vector<glm::vec2>& vertices)
{
	m_vertexCount = static_cast<int>(vertices.size());

	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
		reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	update(vertices);
}

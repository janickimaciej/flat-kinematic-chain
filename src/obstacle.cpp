#include "obstacle.hpp"

#include "shaderPrograms.hpp"

void Obstacle::render(bool selected) const
{
	ShaderPrograms::quad->use();
	ShaderPrograms::quad->setUniform("pos", m_pos);
	ShaderPrograms::quad->setUniform("size", m_size);
	ShaderPrograms::quad->setUniform("selected", selected);
	m_mesh.render();
}

glm::vec2 Obstacle::getPos() const
{
	return m_pos;
}

void Obstacle::setPos(const glm::vec2& pos)
{
	m_pos = pos;
}

glm::vec2 Obstacle::getSize() const
{
	return m_size;
}

void Obstacle::setSize(const glm::vec2& size)
{
	m_size = size;
}

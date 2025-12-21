#pragma once

#include <glm/glm.hpp>

#include <quad.hpp>

class Obstacle
{
public:
	void render(bool selected) const;

	glm::vec2 getPos() const;
	void setPos(const glm::vec2& pos);
	glm::vec2 getSize() const;
	void setSize(const glm::vec2& size);

private:
	glm::vec2 m_pos{0, 0};
	glm::vec2 m_size{1, 1};

	Quad m_mesh{};
};

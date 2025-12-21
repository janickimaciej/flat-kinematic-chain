#pragma once

#include "polylineMesh.hpp"

#include <glm/glm.hpp>

#include <optional>
#include <utility>
#include <vector>

class KinematicChain
{
public:
	struct Configuration
	{
		float angle1Deg;
		float angle2Deg;
	};

	KinematicChain(const glm::vec3& color, float width);
	void render() const;
	bool getIsValid() const;
	void setIsValid(bool isValid);

	glm::vec2 getPos() const;
	void setPos(const glm::vec2& pos);
	static float getLength1();
	static void setLength1(float length);
	static float getLength2();
	static void setLength2(float length);
	Configuration getConfiguration() const;
	void setConfiguration(const Configuration& configuration);

	static std::optional<std::pair<Configuration, Configuration>>
		getInverseSolutions(glm::vec2 pos);

	void updateGeometry();

private:
	bool m_isValid = false;
	glm::vec2 m_pos{};
	static float m_length1;
	static float m_length2;
	Configuration m_configuration{};

	PolylineMesh m_mesh{std::vector<glm::vec2>(3, glm::vec2{})};
	glm::vec3 m_color;
	float m_width;
};

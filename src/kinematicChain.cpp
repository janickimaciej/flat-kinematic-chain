#include "kinematicChain.hpp"

#include "shaderPrograms.hpp"

KinematicChain::KinematicChain(const glm::vec3& color, float width) :
	m_color{color},
	m_width{width}
{ }

void KinematicChain::render() const
{
	ShaderPrograms::polyline->use();
	ShaderPrograms::polyline->setUniform("color", m_color);
	m_mesh.render(m_width);
}

bool KinematicChain::getIsValid() const
{
	return m_isValid;
}

void KinematicChain::setIsValid(bool isValid)
{
	m_isValid = isValid;
}

glm::vec2 KinematicChain::getPos() const
{
	return m_pos;
}

void KinematicChain::setPos(const glm::vec2& pos)
{
	m_pos = pos;
}

float KinematicChain::getLength1()
{
	return m_length1;
}

void KinematicChain::setLength1(float length)
{
	m_length1 = length;
}

float KinematicChain::getLength2()
{
	return m_length2;
}

void KinematicChain::setLength2(float length)
{
	m_length2 = length;
}

KinematicChain::Configuration KinematicChain::getConfiguration() const
{
	return m_configuration;
}

void KinematicChain::setConfiguration(const Configuration& configuration)
{
	m_configuration.angle1Deg = configuration.angle1Deg;
	while (m_configuration.angle1Deg < -180)
	{
		m_configuration.angle1Deg += 360;
	}
	while (m_configuration.angle1Deg > 180)
	{
		m_configuration.angle1Deg -= 360;
	}

	m_configuration.angle2Deg = configuration.angle2Deg;
	while (m_configuration.angle2Deg < -180)
	{
		m_configuration.angle2Deg += 360;
	}
	while (m_configuration.angle2Deg > 180)
	{
		m_configuration.angle2Deg -= 360;
	}

	updateGeometry();
}

std::optional<std::pair<KinematicChain::Configuration, KinematicChain::Configuration>>
	KinematicChain::getInverseSolutions(glm::vec2 pos)
{
	static constexpr float eps = 1e-5f;

	if (pos.x == 0) pos.x = eps;

	float distSquared = pos.x * pos.x + pos.y * pos.y;
	if (distSquared == 0) distSquared = eps;

	float coef = (distSquared - m_length2 * m_length2 + m_length1 * m_length1) / 2.0f;

	float delta = 4 * pos.x * pos.x * (m_length1 * m_length1 * distSquared - coef * coef);
	if (delta < 0)
	{
		return std::nullopt;
	}

	float middleY1 = (2 * coef * pos.y - std::sqrt(delta)) / (2 * distSquared);
	float middleX1 = (coef - pos.y * middleY1) / pos.x;
	float angle11 = std::atan2(middleY1, middleX1);
	float angle21 = std::atan2(pos.y - middleY1, pos.x - middleX1) - angle11;

	float middleY2 = (2 * coef * pos.y + std::sqrt(delta)) / (2 * distSquared);
	float middleX2 = (coef - pos.y * middleY2) / pos.x;
	float angle12 = std::atan2(middleY2, middleX2);
	float angle22 = std::atan2(pos.y - middleY2, pos.x - middleX2) - angle12;

	return std::pair{Configuration{glm::degrees(angle11), glm::degrees(angle21)},
		Configuration{glm::degrees(angle12), glm::degrees(angle22)}};
}

void KinematicChain::updateGeometry()
{
	std::vector<glm::vec2> vertices{};

	vertices.push_back({0, 0});
	float angle1Rad = glm::radians(m_configuration.angle1Deg);
	glm::vec2 middlePoint = m_length1 * glm::vec2{std::cos(angle1Rad), std::sin(angle1Rad)};
	vertices.push_back(middlePoint);
	float angleSumRad = glm::radians(m_configuration.angle1Deg + m_configuration.angle2Deg);
	m_pos = middlePoint + m_length2 * glm::vec2{std::cos(angleSumRad), std::sin(angleSumRad)};
	vertices.push_back(m_pos);

	m_mesh.update(vertices);
}

float KinematicChain::m_length1 = 4;
float KinematicChain::m_length2 = 2;

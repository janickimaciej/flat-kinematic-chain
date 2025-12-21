#include "scene.hpp"

#include "guis/leftPanel.hpp"
#include "shaderPrograms.hpp"

Scene::Scene(const glm::ivec2& viewportSize) :
	m_viewportSize{viewportSize}
{
	updateConfigurationSpace();
	setMainChainPos({3, 4});
	cancel();
}

void Scene::update()
{
	m_animation.update();
}

void Scene::render()
{
	static constexpr glm::vec3 backgroundColor{0.1f, 0.1f, 0.1f};
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderPrograms::quad->use();
	ShaderPrograms::quad->setUniform("viewportSize", m_viewportSize);

	ShaderPrograms::polyline->use();
	ShaderPrograms::polyline->setUniform("viewportSize", m_viewportSize);

	for (const auto& obstacle : m_obstacles)
	{
		obstacle->render(obstacle.get() == m_selectedObstacle);
	}

	if (m_doubleSolution)
	{
		m_redChain.render();
		m_greenChain.render();
	}

	if (m_mode == Mode::edit)
	{
		if (m_mainChain.getIsValid() && !m_doubleSolution) m_mainChain.render();
	}
	else
	{
		if (m_startChain.getIsValid() && (!m_doubleSolution || m_setPosMode != SetPosMode::start))
		{
			m_startChain.render();
		}
		if (m_currChain.getIsValid() && !m_doubleSolution)
		{
			m_currChain.render();
		}
		if (m_endChain.getIsValid() && (!m_doubleSolution || m_setPosMode != SetPosMode::end))
		{
			m_endChain.render();
		}
	}
}

Mode Scene::getMode() const
{
	return m_mode;
}

void Scene::setMode(Mode mode)
{
	cancel();
	m_startChain.setIsValid(false);
	m_endChain.setIsValid(false);
	updateCurrChain();
	m_mode = mode;
}

void Scene::setSetPosMode(SetPosMode mode)
{
	cancel();
	m_setPosMode = mode;
}

unsigned int Scene::getConfigurationSpaceTextureId() const
{
	return m_configurationSpaceFramebuffer.getTextureId();
}

void Scene::updateConfigurationSpace()
{
	m_configurationSpaceFramebuffer.bind();
	static constexpr glm::vec3 backgroundColor{0.3f, 0.3f, 0.3f};
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderPrograms::configurationSpace->use();
	ShaderPrograms::configurationSpace->setUniform("kinematicChainLength1",
		KinematicChain::getLength1());
	ShaderPrograms::configurationSpace->setUniform("kinematicChainLength2",
		KinematicChain::getLength2());
	for (const auto& obstacle : m_obstacles)
	{
		ShaderPrograms::configurationSpace->setUniform("obstaclePos", obstacle->getPos());
		ShaderPrograms::configurationSpace->setUniform("obstacleSize", obstacle->getSize());
		m_quad.render();
	}

	m_configurationSpaceFramebuffer.unbind();

	updateConfigurationSpaceData();

	setMainChainPos(getMainChainPos());
}

float Scene::getLength1() const
{
	return KinematicChain::getLength1();
}

void Scene::setLength1(float length)
{
	cancel();
	KinematicChain::setLength1(length);
	m_mainChain.updateGeometry();
}

float Scene::getLength2() const
{
	return KinematicChain::getLength2();
}

void Scene::setLength2(float length)
{
	cancel();
	KinematicChain::setLength2(length);
	m_mainChain.updateGeometry();
}

glm::vec2 Scene::getMainChainPos() const
{
	return m_mainChain.getPos();
}

void Scene::setMainChainPos(const glm::vec2& pos)
{
	setChainPos(m_mainChain, pos);
}

glm::vec2 Scene::getStartChainPos() const
{
	return m_startChain.getPos();
}

void Scene::setStartChainPos(const glm::vec2& pos)
{
	m_setPosMode = SetPosMode::start;
	setChainPos(m_startChain, pos);
	updateCurrChain();
}

glm::vec2 Scene::getCurrChainPos() const
{
	return m_currChain.getPos();
}

glm::vec2 Scene::getEndChainPos() const
{
	return m_endChain.getPos();
}

void Scene::setEndChainPos(const glm::vec2& pos)
{
	m_setPosMode = SetPosMode::end;
	setChainPos(m_endChain, pos);
	updateCurrChain();
}

void Scene::setChainScreenPos(const glm::vec2& screenPos)
{
	glm::vec2 pos =
		(screenPos - glm::vec2{LeftPanel::width, 0} - glm::vec2{m_viewportSize} / 2.0f) * pixSize;
	pos.y *= -1;

	if (m_mode == Mode::edit)
	{
		setMainChainPos(pos);
	}
	else
	{
		if (m_setPosMode == SetPosMode::start)
		{
			setStartChainPos(pos);
		}
		else if (m_setPosMode == SetPosMode::end)
		{
			setEndChainPos(pos);
		}
	}
}

bool Scene::doubleSolution() const
{
	return m_doubleSolution;
}

void Scene::chooseRed()
{
	chooseColor(m_redChain.getConfiguration());
}

void Scene::chooseGreen()
{
	chooseColor(m_greenChain.getConfiguration());
}

void Scene::selectObstacle(const glm::vec2& screenPos)
{
	std::optional<int> index = std::nullopt;
	static constexpr float treshold = 30;
	float minScreenDistanceSquared = treshold * treshold;

	for (int i = 0; i < m_obstacles.size(); ++i)
	{
		glm::vec2 obstaclePos = m_obstacles[i]->getPos();
		glm::vec2 obstacleScreenPos = glm::vec2{obstaclePos.x, -obstaclePos.y} / pixSize +
			glm::vec2{m_viewportSize} / 2.0f + glm::vec2{LeftPanel::width, 0};
		glm::vec2 relativePos = obstacleScreenPos - screenPos;
		float screenDistanceSquared = glm::dot(relativePos, relativePos);
		if (screenDistanceSquared < minScreenDistanceSquared)
		{
			index = i;
			minScreenDistanceSquared = screenDistanceSquared;
		}
	}

	if (index.has_value())
	{
		m_selectedObstacle = m_obstacles[*index].get();
	}
}

void Scene::addObstacle()
{
	m_obstacles.push_back(std::make_unique<Obstacle>());
	m_selectedObstacle = m_obstacles.back().get();
}

void Scene::deleteSelectedObstacle()
{
	std::erase_if
	(
		m_obstacles,
		[this] (const std::unique_ptr<Obstacle>& obstacle)
		{
			return obstacle.get() == m_selectedObstacle;
		}
	);

	m_selectedObstacle = nullptr;
}

void Scene::cancel()
{
	m_selectedObstacle = nullptr;
	m_doubleSolution = false;
	m_setPosMode = SetPosMode::none;
}

Obstacle* Scene::getSelectedObstacle()
{
	return m_selectedObstacle;
}

void Scene::startAnimation()
{
	m_animation.start();
}

void Scene::stopAnimation()
{
	m_animation.stop();
}

void Scene::resetAnimation()
{
	m_animation.reset();
}

float Scene::getAnimationTime() const
{
	return m_animation.getEndTime();
}

void Scene::setAnimationTime(float time)
{
	m_animation.setEndTime(time);
}

float Scene::getTime() const
{
	return m_animation.getTime();
}

void Scene::setChainPos(KinematicChain& chain, const glm::vec2& pos)
{
	chain.setPos(pos);

	auto solutions = KinematicChain::getInverseSolutions(pos);
	if (!solutions.has_value())
	{
		chain.setIsValid(false);
		m_doubleSolution = false;
		return;
	}

	m_redChain.setConfiguration(solutions->first);
	m_greenChain.setConfiguration(solutions->second);

	bool redValid = !intersectsObstacle(m_redChain.getConfiguration());
	bool greenValid = !intersectsObstacle(m_greenChain.getConfiguration());

	chain.setConfiguration(redValid ? solutions->first : solutions->second);

	chain.setIsValid(redValid || greenValid);
	m_doubleSolution = redValid && greenValid;
}

void Scene::chooseColor(const KinematicChain::Configuration& configuration)
{
	if (m_mode == Mode::edit)
	{
		m_mainChain.setConfiguration(configuration);
	}
	else if (m_setPosMode == SetPosMode::start)
	{
		m_startChain.setConfiguration(configuration);
	}
	else if (m_setPosMode == SetPosMode::end)
	{
		m_endChain.setConfiguration(configuration);
	}

	updateCurrChain();
	cancel();
}

void Scene::updateCurrChain()
{
	bool bothChainsValid = m_startChain.getIsValid() && m_endChain.getIsValid();
	bool pathExists = true; // TODO
	m_currChain.setIsValid(bothChainsValid && pathExists);
	if (!bothChainsValid)
	{
		return;
	}

	m_currChain.setPos(m_startChain.getPos()); // TODO
	m_currChain.setConfiguration(m_startChain.getConfiguration()); // TODO
}

void Scene::updateConfigurationSpaceData()
{
	m_configurationSpaceFramebuffer.bind();
	m_configurationSpaceFramebuffer.getTextureData((*m_configurationSpaceData)[0][0].data());
	m_configurationSpaceFramebuffer.unbind();
}

bool Scene::intersectsObstacle(const KinematicChain::Configuration& configuration) const
{
	float xPix = configuration.angle1Deg + 180.0f - 0.5f;
	float yPix = configuration.angle2Deg + 180.0f - 0.5f;

	int xPixInd = std::max(static_cast<int>(std::round(xPix)), 0);
	int yPixInd = std::max(static_cast<int>(std::round(yPix)), 0);

	return (*m_configurationSpaceData)[yPixInd][xPixInd][0] > 128 &&
		(*m_configurationSpaceData)[yPixInd][xPixInd][1] < 1 &&
		(*m_configurationSpaceData)[yPixInd][xPixInd][2] < 1;
}

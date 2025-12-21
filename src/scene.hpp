#pragma once

#include "animation.hpp"
#include "framebuffer.hpp"
#include "kinematicChain.hpp"
#include "mode.hpp"
#include "obstacle.hpp"
#include "quad.hpp"
#include "setPosMode.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Scene
{
public:
	Scene(const glm::ivec2& windowSize);
	void update();
	void render();
	
	Mode getMode() const;
	void setMode(Mode mode);
	void setSetPosMode(SetPosMode mode);

	void updateConfigurationSpace();
	unsigned int getConfigurationSpaceTextureId() const;

	float getLength1() const;
	void setLength1(float length);
	float getLength2() const;
	void setLength2(float length);
	
	glm::vec2 getMainChainPos() const;
	void setMainChainPos(const glm::vec2& pos);
	glm::vec2 getStartChainPos() const;
	void setStartChainPos(const glm::vec2& pos);
	glm::vec2 getCurrChainPos() const;
	glm::vec2 getEndChainPos() const;
	void setEndChainPos(const glm::vec2& pos);
	void setChainScreenPos(const glm::vec2& screenPos);
	bool doubleSolution() const;
	void chooseRed();
	void chooseGreen();

	void selectObstacle(const glm::vec2& screenPos);
	void addObstacle();
	void deleteSelectedObstacle();
	void cancel();
	Obstacle* getSelectedObstacle();
	
	void startAnimation();
	void stopAnimation();
	void resetAnimation();

	float getAnimationTime() const;
	void setAnimationTime(float time);
	float getTime() const;

private:
	using ConfigurationSpaceData = std::array<std::array<std::array<unsigned char, 3>, 360>, 360>;

	const glm::ivec2& m_viewportSize{};
	Mode m_mode = Mode::edit;
	SetPosMode m_setPosMode = SetPosMode::none;
	bool m_doubleSolution{};
	
	static constexpr float pixSize = 0.01f;
	static constexpr glm::vec3 white{1, 1, 1};
	static constexpr glm::vec3 red{1, 0, 0};
	static constexpr glm::vec3 green{0, 1, 0};

	KinematicChain m_mainChain{white, 3};
	KinematicChain m_startChain{white, 1};
	KinematicChain m_currChain{white, 3};
	KinematicChain m_endChain{white, 1};

	KinematicChain m_redChain{red, 3};
	KinematicChain m_greenChain{green, 3};

	std::vector<std::unique_ptr<Obstacle>> m_obstacles{};
	Obstacle* m_selectedObstacle = nullptr;

	Framebuffer m_configurationSpaceFramebuffer{{360, 360}};
	Quad m_quad{};
	std::unique_ptr<ConfigurationSpaceData> m_configurationSpaceData =
		std::make_unique<ConfigurationSpaceData>();

	Animation m_animation{[this] () { updateCurrChain(); }};

	void setChainPos(KinematicChain& chain, const glm::vec2& pos);
	void chooseColor(const KinematicChain::Configuration& configuration);
	void updateCurrChain();
	void updateConfigurationSpaceData();
	bool intersectsObstacle(const KinematicChain::Configuration& configuration) const;
};

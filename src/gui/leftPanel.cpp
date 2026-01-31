#include "gui/leftPanel.hpp"

#include "gui/configurationSpacePanel.hpp"
#include "mode.hpp"

#include <imgui/imgui.h>

#include <algorithm>

LeftPanel::LeftPanel(Scene& scene, const glm::ivec2& viewportSize) :
	m_scene{scene},
	m_viewportSize{viewportSize}
{ }

void LeftPanel::update()
{
	ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
	ImGui::SetNextWindowSize({width, static_cast<float>(m_viewportSize.y -
		ConfigurationSpacePanel::size + 1)}, ImGuiCond_Always);
	ImGui::Begin("leftPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::PushItemWidth(100);

	updateMode();

	ImGui::Spacing();

	if (ImGui::Button("Update configuration space"))
	{
		m_scene.updateConfigurationSpace();
	}

	ImGui::SeparatorText("Kinematic chain");

	if (m_scene.getMode() == Mode::edit)
	{
		ImGui::Text("Lengths");

		updateKinematicChainLengths();

		ImGui::Text("Position");
		updateKinematicChainPos(
			[this] () { return m_scene.getMainChainPos(); },
			[this] (const glm::vec2& pos) { m_scene.setMainChainPos(pos); },
			"##mainChain");
	}
	else
	{
		ImGui::Text("Start position");
		updateKinematicChainPos(
			[this] () { return m_scene.getStartChainPos(); },
			[this] (const glm::vec2& pos) { m_scene.setStartChainPos(pos); },
			"##startChain");

		ImGui::Spacing();

		ImGui::Text("Current position");
		updateKinematicChainPos(
			[this] () { return m_scene.getCurrChainPos(); },
			[this] (const glm::vec2&) { },
			"##currChain",
			true);

		ImGui::Spacing();

		ImGui::Text("End position");
		updateKinematicChainPos(
			[this] () { return m_scene.getEndChainPos(); },
			[this] (const glm::vec2& pos) { m_scene.setEndChainPos(pos); },
			"##endChain");

		ImGui::Spacing();

		if (ImGui::Button("Set start position"))
		{
			m_scene.setSetPosMode(SetPosMode::start);
		}

		ImGui::Spacing();

		if (ImGui::Button("Set end position"))
		{
			m_scene.setSetPosMode(SetPosMode::end);
		}
	}

	if (m_scene.doubleSolution())
	{
		ImGui::Spacing();

		if (ImGui::Button("Choose red"))
		{
			m_scene.chooseRed();
		}
		ImGui::SameLine();
		if (ImGui::Button("Choose green"))
		{
			m_scene.chooseGreen();
		}
	}

	if (m_scene.getMode() == Mode::edit)
	{
		ImGui::SeparatorText("Obstacles");

		if (ImGui::Button("Add", {70, 0}))
		{
			m_scene.addObstacle();
		}

		ImGui::Spacing();

		if (m_scene.isObstacleSelected())
		{
			updateObstacle();
			ImGui::Spacing();
			if (ImGui::Button("Delete selected"))
			{
				m_scene.deleteSelectedObstacle();
			}
		}
	}
	else
	{
		ImGui::SeparatorText("Animation");
		updateAnimationTime();
		ImGui::Spacing();
		updateAnimationButtons();
		ImGui::Spacing();
		updateTime();
	}

	ImGui::PopItemWidth();
	ImGui::End();
}

void LeftPanel::updateMode()
{
	Mode mode = m_scene.getMode();
	if (ImGui::BeginCombo("##mode",
		modeLabels[static_cast<int>(mode)].c_str()))
	{
		for (int i = 0; i < 2; ++i)
		{
			bool isSelected = i == static_cast<int>(mode);
			if (ImGui::Selectable(modeLabels[i].c_str(), isSelected))
			{
				m_scene.setMode(static_cast<Mode>(i));
			}
		}
		ImGui::EndCombo();
	}
}

void LeftPanel::updateKinematicChainLengths()
{
	static const std::string suffix = "##lengths";
	static const std::string format = "%.2f";
	constexpr float dragSpeed = 0.01f;

	ImGui::PushItemWidth(105);

	float length1 = m_scene.getLength1();
	float prevLength1 = length1;

	float length2 = m_scene.getLength2();
	float prevLength2 = length2;

	ImGui::DragFloat(("L1" + suffix).c_str(), &length1, dragSpeed,
		0.01f, 100.0f, format.c_str(), ImGuiSliderFlags_AlwaysClamp);
	ImGui::SameLine();
	ImGui::DragFloat(("L2" + suffix).c_str(), &length2, dragSpeed,
		0.01f, 100.0f, format.c_str(), ImGuiSliderFlags_AlwaysClamp);

	if (length1 != prevLength1)
	{
		m_scene.setLength1(length1);
	}

	if (length2 != prevLength2)
	{
		m_scene.setLength2(length2);
	}

	ImGui::PopItemWidth();
}

void LeftPanel::updateKinematicChainPos(const std::function<glm::vec2(void)>& getter,
	const std::function<void(const glm::vec2&)>& setter, const std::string& suffix, bool disabled)
{
	static const std::string format = "%.2f";
	constexpr float dragSpeed = 0.01f;

	ImGui::PushItemWidth(110);

	glm::vec2 pos = getter();
	glm::vec2 prevPos = pos;

	int flags = ImGuiSliderFlags_AlwaysClamp;
	if (disabled) flags = flags | ImGuiSliderFlags_NoInput;
	ImGui::DragFloat(("x" + suffix).c_str(), &pos.x, dragSpeed, pos.x, pos.x, format.c_str(),
		flags);
	ImGui::SameLine();
	ImGui::DragFloat(("y" + suffix).c_str(), &pos.y, dragSpeed, pos.y, pos.y, format.c_str(),
		flags);

	if (!disabled && pos != prevPos)
	{
		setter(pos);
	}

	ImGui::PopItemWidth();
}

void LeftPanel::updateObstacle()
{
	static const std::string suffix = "##obstacle";
	static const std::string format = "%.2f";
	constexpr float dragSpeed = 0.02f;

	ImGui::PushItemWidth(110);

	glm::vec2 pos = m_scene.getSelectedObstaclePos();
	glm::vec2 prevPos = pos;

	ImGui::Text("Position");
	ImGui::DragFloat(("x" + suffix).c_str(), &pos.x, dragSpeed, -100.0f, 100.0f,
		format.c_str(), ImGuiSliderFlags_AlwaysClamp);
	ImGui::SameLine();
	ImGui::DragFloat(("y" + suffix).c_str(), &pos.y, dragSpeed, -100.0f, 100.0f,
		format.c_str(), ImGuiSliderFlags_AlwaysClamp);

	if (pos != prevPos)
	{
		m_scene.setSelectedObstaclePos(pos);
	}

	ImGui::PopItemWidth();
	ImGui::PushItemWidth(80);

	glm::vec2 size = m_scene.getSelectedObstacleSize();
	glm::vec2 prevSize = size;

	ImGui::Text("Size");
	ImGui::DragFloat(("width" + suffix).c_str(), &size.x, dragSpeed, 0.01f, 100.0f,
		format.c_str(), ImGuiSliderFlags_AlwaysClamp);
	ImGui::SameLine();
	ImGui::DragFloat(("height" + suffix).c_str(), &size.y, dragSpeed, 0.01f, 100.0f,
		format.c_str(), ImGuiSliderFlags_AlwaysClamp);

	if (size != prevSize)
	{
		m_scene.setSelectedObstacleSize(size);
	}

	ImGui::PopItemWidth();
}

void LeftPanel::updateAnimationTime()
{
	float animationTime = m_scene.getAnimationTime();
	float prevAnimationTime = animationTime;

	constexpr float speed = 0.1f;
	ImGui::Text("Time");
	ImGui::DragFloat("##animationTime", &animationTime, speed, 0.01f, 3600.0f, "%.2f",
		ImGuiSliderFlags_AlwaysClamp);

	if (animationTime != prevAnimationTime)
	{
		m_scene.setAnimationTime(animationTime);
	}
}

void LeftPanel::updateAnimationButtons()
{
	if (ImGui::Button("Start"))
	{
		m_scene.startAnimation();
	}

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
	{
		m_scene.stopAnimation();
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		m_scene.resetAnimation();
	}
}

void LeftPanel::updateTime()
{
	ImGui::Text("t = %.2f s", m_scene.getTime());
}

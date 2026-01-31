#include "gui/configurationSpacePanel.hpp"

#include <imgui/imgui.h>

ConfigurationSpacePanel::ConfigurationSpacePanel(const Scene& scene,
	const glm::ivec2& viewportSize) :
	m_scene{scene},
	m_viewportSize{viewportSize}
{ }

void ConfigurationSpacePanel::update()
{
	ImGui::SetNextWindowPos({0, static_cast<float>(m_viewportSize.y - size)}, ImGuiCond_Always);
	ImGui::SetNextWindowSize({size, size}, ImGuiCond_Always);
	ImGui::Begin("configurationSpacePanel", nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	static constexpr int imageSize = 360;
	ImGui::Image
	(
		reinterpret_cast<void*>(static_cast<std::intptr_t>(
			m_scene.getConfigurationSpaceTextureId())),
		{imageSize, imageSize},
		ImVec2{0.0f, 1.0f},
		ImVec2{1.0f, 0.0f}
	);

	ImGui::End();
}

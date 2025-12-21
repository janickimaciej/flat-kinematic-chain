#include "shaderPrograms.hpp"

#include <string>

namespace ShaderPrograms
{
	std::string path(const std::string& shaderName);

	std::unique_ptr<const ShaderProgram> quad{};
	std::unique_ptr<const ShaderProgram> polyline{};
	std::unique_ptr<const ShaderProgram> configurationSpace{};

	void init()
	{
		quad = std::make_unique<const ShaderProgram>(path("quadVS"), path("quadFS"));
		polyline = std::make_unique<const ShaderProgram>(path("polylineVS"), path("polylineFS"));
		configurationSpace = std::make_unique<const ShaderProgram>(path("configurationSpaceVS"),
			path("configurationSpaceFS"));
	}

	std::string path(const std::string& shaderName)
	{
		return "src/shaders/" + shaderName + ".glsl";
	}
}

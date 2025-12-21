#include "shaderPrograms.hpp"

#include <string>

namespace ShaderPrograms
{
	std::string path(const std::string& shaderName);

	std::unique_ptr<const ShaderProgram> quad{};
	std::unique_ptr<const ShaderProgram> polyline{};

	void init()
	{
		quad = std::make_unique<const ShaderProgram>(path("quadVS"), path("quadFS"));
		polyline = std::make_unique<const ShaderProgram>(path("polylineVS"), path("polylineFS"));
	}

	std::string path(const std::string& shaderName)
	{
		return "src/shaders/" + shaderName + ".glsl";
	}
}

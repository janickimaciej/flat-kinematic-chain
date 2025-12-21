#pragma once

#include "shaderProgram.hpp"

#include <memory>

namespace ShaderPrograms
{
	void init();

	extern std::unique_ptr<const ShaderProgram> quad;
	extern std::unique_ptr<const ShaderProgram> polyline;
}

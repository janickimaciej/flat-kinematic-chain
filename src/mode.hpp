#pragma once

#include <array>
#include <string>

enum class Mode
{
	edit,
	path
};

inline const std::array<std::string, 2> modeLabels
{
	"Edit",
	"Path"
};

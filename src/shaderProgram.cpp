#include "shaderProgram.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>

static constexpr std::size_t errorLogSize = 512;

ShaderProgram::ShaderProgram(const std::string& vertexShaderPath,
	const std::string& fragmentShaderPath) :
	ShaderProgram
	{
		{vertexShaderPath, fragmentShaderPath},
		{GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}
	}
{ }

ShaderProgram::ShaderProgram(const std::string& vertexShaderPath,
	const std::string& geometryShaderPath, const std::string& fragmentShaderPath) :
	ShaderProgram
	{
		{vertexShaderPath, geometryShaderPath, fragmentShaderPath},
		{GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER}
	}
{ }

ShaderProgram::ShaderProgram(const std::string& vertexShaderPath,
	const std::string& tessCtrlShaderPath, const std::string& tessEvalShaderPath,
	const std::string& fragmentShaderPath) :
	ShaderProgram
	{
		{vertexShaderPath, tessCtrlShaderPath, tessEvalShaderPath, fragmentShaderPath},
		{GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_FRAGMENT_SHADER}
	}
{ }

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_id);
}

void ShaderProgram::use() const
{
	glUseProgram(m_id);
}

void ShaderProgram::setUniform(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), static_cast<int>(value));
}

void ShaderProgram::setUniform(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::ivec2& value) const
{
	glUniform2iv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE,
		glm::value_ptr(value));
}

ShaderProgram::ShaderProgram(const std::vector<std::string>& shaderPaths,
	const std::vector<GLenum>& shaderTypes)
{
	std::vector<unsigned int> shaders{};
	for (int i = 0; i < shaderPaths.size(); ++i)
	{
		shaders.push_back(createShader(shaderPaths[i], shaderTypes[i]));
	}
	m_id = createShaderProgram(shaders);
	deleteShaders(shaders);
}

unsigned int ShaderProgram::createShader(const std::string& shaderPath, GLenum shaderType)
{
	std::string shaderCode = readShaderFile(shaderPath);

	unsigned int shader = glCreateShader(shaderType);
	const char* shaderCodeCStr = shaderCode.c_str();
	glShaderSource(shader, 1, &shaderCodeCStr, NULL);
	glCompileShader(shader);
	int success{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		printCompilationError(shader, shaderType);
	}
	return shader;
}

unsigned int ShaderProgram::createShaderProgram(const std::vector<unsigned int>& shaders)
{
	unsigned int shaderProgram = glCreateProgram();
	for (unsigned int shader : shaders)
	{
		glAttachShader(shaderProgram, shader);
	}
	glLinkProgram(shaderProgram);
	int success{};
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		printLinkingError(shaderProgram);
	}
	return shaderProgram;
}

void ShaderProgram::deleteShaders(const std::vector<unsigned int>& shaders)
{
	for (unsigned int shader : shaders)
	{
		glDeleteShader(shader);
	}
}

std::string ShaderProgram::readShaderFile(const std::string& shaderPath)
{
	std::string shaderCode{};

	try
	{
		std::ifstream file{};
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		std::stringstream stream{};
		file.open(shaderPath);
		stream << file.rdbuf();
		file.close();
		shaderCode = stream.str();
	}
	catch (std::ifstream::failure)
	{
		std::cerr << "Error reading file:\n" << shaderPath << '\n';
	}

	return shaderCode;
}

void ShaderProgram::printCompilationError(unsigned int shader, GLenum shaderType)
{
	std::array<char, errorLogSize> errorLog{};
	glGetShaderInfoLog(shader, errorLogSize, nullptr, errorLog.data());
	std::string shaderTypeName{};
	switch (shaderType)
	{
		case GL_VERTEX_SHADER:
			shaderTypeName = "vertex";
			break;

		case GL_TESS_CONTROL_SHADER:
			shaderTypeName = "tessellation control";
			break;

		case GL_TESS_EVALUATION_SHADER:
			shaderTypeName = "tessellation evaluation";
			break;

		case GL_GEOMETRY_SHADER:
			shaderTypeName = "geometry";
			break;

		case GL_FRAGMENT_SHADER:
			shaderTypeName = "fragment";
			break;
	}
	std::cerr << "Error compiling " + shaderTypeName + " shader:\n" << errorLog.data() << '\n';
}

void ShaderProgram::printLinkingError(unsigned int shaderProgram)
{
	std::array<char, errorLogSize> errorLog{};
	glGetProgramInfoLog(shaderProgram, errorLogSize, nullptr, errorLog.data());
	std::cerr << "Error linking shader program:\n" << errorLog.data() << '\n';
}

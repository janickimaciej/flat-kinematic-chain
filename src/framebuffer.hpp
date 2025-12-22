#pragma once

#include <glm/glm.hpp>

#include <array>

class Framebuffer
{
public:
	Framebuffer(const glm::ivec2& size);
	~Framebuffer();
	void bind();
	void unbind() const;
	void bindTexture() const;
	void resize(const glm::ivec2& size) const;
	unsigned int getTextureId() const;
	void getTextureData(unsigned char* output) const;
	void setTextureData(unsigned char* input) const;

private:
	unsigned int m_FBO{};
	unsigned int m_colorBuffer{};
	unsigned int m_depthStencilBuffer{};
	glm::ivec2 m_size{};
	std::array<int, 4> m_previousViewport{};

	void createColorBuffer(const glm::ivec2& size);
	void createDepthStencilBuffer(const glm::ivec2& size);

	void resizeColorBuffer(const glm::ivec2& size) const;
	void resizeDepthStencilBuffer(const glm::ivec2& size) const;
};

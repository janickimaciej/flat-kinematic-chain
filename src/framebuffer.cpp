#include "framebuffer.hpp"

#include <glad/glad.h>

Framebuffer::Framebuffer(const glm::ivec2& size) :
	m_size{size}
{
	glGenFramebuffers(1, &m_FBO);
	bind();

	createColorBuffer(size);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

	createDepthStencilBuffer(size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
		m_depthStencilBuffer);

	unbind();
}

Framebuffer::~Framebuffer()
{
	glDeleteRenderbuffers(1, &m_depthStencilBuffer);
	glDeleteTextures(1, &m_colorBuffer);
	glDeleteFramebuffers(1, &m_FBO);
}

void Framebuffer::bind()
{
	glGetIntegerv(GL_VIEWPORT, m_previousViewport.data());
	glViewport(0, 0, m_size.x, m_size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void Framebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(m_previousViewport[0], m_previousViewport[1], m_previousViewport[2],
		m_previousViewport[3]);
}

void Framebuffer::bindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
}

void Framebuffer::resize(const glm::ivec2& size) const
{
	resizeColorBuffer(size);
	resizeDepthStencilBuffer(size);
}

unsigned int Framebuffer::getTextureId() const
{
	return m_colorBuffer;
}

void Framebuffer::getTextureData(unsigned char* output) const
{
	glReadPixels(0, 0, m_size.x, m_size.y, GL_RGB, GL_UNSIGNED_BYTE, output);
}

void Framebuffer::setTextureData(unsigned char* input) const
{
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_size.x, m_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, input);
}

void Framebuffer::createColorBuffer(const glm::ivec2& size)
{
	glGenTextures(1, &m_colorBuffer);
	resizeColorBuffer(size);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::createDepthStencilBuffer(const glm::ivec2& size)
{
	glGenRenderbuffers(1, &m_depthStencilBuffer);
	resizeDepthStencilBuffer(size);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::resizeColorBuffer(const glm::ivec2& size) const
{
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE,
		nullptr);
}

void Framebuffer::resizeDepthStencilBuffer(const glm::ivec2& size) const
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
}

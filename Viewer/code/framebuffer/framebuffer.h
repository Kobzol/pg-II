#pragma once

#include <glew.h>

#include <memory>


class Framebuffer
{
public:
	GLuint framebuffer;
	GLuint texture;

	Framebuffer();

	void bind();
	void unbind();

	void createAttachments(int width, int height);
	void createDepthAttachment(int width, int height);

	void setViewport();

private:
	int width, height;
};

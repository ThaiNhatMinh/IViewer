#pragma once
class FrameBuffer
{
public:
	FrameBuffer(GLuint w,GLuint h, GLenum internalformat, GLenum format);
	~FrameBuffer();

	void Bind();
	GLuint FBO;
	Texture Tex;
};


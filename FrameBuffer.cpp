#include "stdafx.h"
#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(GLuint w, GLuint h,GLenum internalformat,GLenum format)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &Tex.iIndex);
	glBindTexture(GL_TEXTURE_2D, Tex.iIndex);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, w, h, 0, format, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Tex.iIndex, 0);
	Tex.iHeight = h;
	Tex.iWidth = w;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1,&FBO);

}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

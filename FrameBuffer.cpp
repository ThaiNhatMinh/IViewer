#include "stdafx.h"
#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(GLuint w, GLuint h,GLenum internalformat,GLenum format)
{
	glGenTextures(1, &iIndex);
	glBindTexture(GL_TEXTURE_2D, iIndex);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &depth);
	glBindRenderbuffer(GL_RENDERBUFFER, depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, iIndex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Framebuffer not complete.";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	iWidth = w;
	iHeight = h;
	iBpp = internalformat;
	iType = format;
}


FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1,&FBO);

}

void FrameBuffer::Resize(int w, int h)
{
	glBindTexture(GL_TEXTURE_2D, iIndex);
	glTexImage2D(GL_TEXTURE_2D, 0, iBpp, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	iWidth = w;
	iHeight = h;
}

void FrameBuffer::Bind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

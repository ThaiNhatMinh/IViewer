#pragma once
class FrameBuffer
{
public:
	FrameBuffer() {};
	FrameBuffer(GLuint w,GLuint h, GLenum internalformat, GLenum format);
	~FrameBuffer();

	FrameBuffer& operator=(const FrameBuffer& other) = delete;
	FrameBuffer(const FrameBuffer& other) = delete;
	FrameBuffer(FrameBuffer&& other)
	{
		FBO = other.FBO;
		iIndex = other.iIndex;
		iWidth = other.iWidth;
		iHeight = other.iHeight;
		depth = other.depth;
		other.FBO = 0;
		other.iIndex = 0;
		other.depth = 0;
	}
	FrameBuffer& operator=(FrameBuffer&& other)
	{
		if (this != &other)
		{
			FBO = other.FBO;
			iIndex = other.iIndex;
			iWidth = other.iWidth;
			iHeight = other.iHeight;
			depth = other.depth;
			other.FBO = 0;
			other.iIndex = 0;
			other.depth = 0;
		}

		return *this;
	}
	void Bind();
	void UnBind();
	GLuint FBO;
	GLuint iIndex;
	GLuint depth;
	GLuint iWidth;
	GLuint iHeight;
};


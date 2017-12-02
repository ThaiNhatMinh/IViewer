#include "stdafx.h"
#include "Image.h"
#include <IL\il.h>
#include <IL\ilu.h>

Image::Image()
{
	m_Shader = std::unique_ptr<Shader>(new Shader("Data\\Shader\\Quad.vs", "Data\\Shader\\Quad.fs"));
	m_Quad.Finalize(m_Shader.get());
}

void Image::LoadTexture(const char * file)
{
	GLint width, height, iType, iBpp;

	
	ilLoadImage(file);
	ILenum Error;
	Error = ilGetError();

	if (Error != IL_NO_ERROR)
	{
		cout << "ERROR:LOADIMAGE\n";
		if (Error == IL_FILE_READ_ERROR)
			cout << "FILE_READ_ERROR\n";
		return;
	}

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	iType = ilGetInteger(IL_IMAGE_FORMAT);
	ILubyte *Data = ilGetData();
	iBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, iBpp, width, height, 0, iType, GL_UNSIGNED_BYTE, Data);
	glBindTexture(GL_TEXTURE_2D, 0);
	

	ilResetMemory();

	sprintf(m_Tex.szName, "%s", file);
	m_Tex.iIndex = id;
	m_Tex.iWidth = width;
	m_Tex.iHeight = height;

	FrameBuffer fb(width, height, iBpp, iType);
	m_List.push_back(fb);
	m_Quad.Resize(width, height);
}


Image::~Image()
{
}

bool Image::HasTexture()
{
	return m_List.size() > 0;
}
void Image::Render()
{
	m_Shader->Use();
	
	glBindTexture(GL_TEXTURE_2D, m_Tex.iIndex);
	glBindVertexArray(m_Quad.VAO);
	glDrawArrays(m_Quad.Topology, 0, 4);
}

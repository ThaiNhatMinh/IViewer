#include "stdafx.h"
#include "Image.h"
#include <FreeImage.h>
Image::Image(vec2 windowsize)
{
	m_Shader = std::unique_ptr<Shader>(new Shader("Data\\Shader\\Quad.vs", "Data\\Shader\\Quad.fs"));
	m_Quad.Finalize(m_Shader.get());
	m_WinSize = windowsize;

}

void Image::LoadTexture(const char * file)
{
	

	
	FREE_IMAGE_FORMAT img_format = FreeImage_GetFileType(file, 0);
	if (img_format == FREE_IMAGE_FORMAT::FIF_UNKNOWN)
	{
		printf("Error: Unknown format !");
		return ;
	}

	FIBITMAP* img_bm = FreeImage_Load(img_format, file);
	if (img_bm == NULL)
	{
		printf("Error: Image load FAIL !");
		return;
	}

	iWidth = FreeImage_GetWidth(img_bm);
	iHeight = FreeImage_GetHeight(img_bm);
	GLuint bpp = FreeImage_GetBPP(img_bm);
	iType = FreeImage_GetColorsUsed(img_bm);
	char *pixels = (char*)FreeImage_GetBits(img_bm);

	//iBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
	if (bpp == 24) iBpp = GL_RGB;
	else if (bpp == 32) iBpp = GL_RGBA;
	iType = GL_RGB;
	FrameBuffer fb(iWidth, iHeight, iBpp, iType);
	m_List.push_back(std::move(fb));

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, iBpp, iWidth, iHeight, 0, iType, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	


	
	iIndex = id;

	
	m_Quad.Resize(iWidth, iHeight);
	hasload = 1;
	
	FreeImage_Unload(img_bm);


}

bool Image::SaveFile(const char * file)
{
	FrameBuffer& fb = m_List.back();
	
	Render();
	GLint bpp;
	if (iBpp == GL_RGB) bpp = 24;
	else if (iBpp == GL_RGBA) bpp = 32;
	FIBITMAP* img_bm = FreeImage_Allocate(iWidth, iHeight, 32);
	int* pixels = (int*)FreeImage_GetBits(img_bm);

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, iWidth, iHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	if (FreeImage_Save(FREE_IMAGE_FORMAT::FIF_PNG, img_bm, file))
	{
		printf("Info: Save success !!");
	}
	else
	{
		printf("Info: Save fail !!");
	}

	// Clean up
	FreeImage_Unload(img_bm);
	return true;
}


Image::~Image()
{
}

bool Image::HasTexture()
{
	return hasload;
}
GLuint Image::Render()
{
	FrameBuffer& fb = m_List.back();
	fb.Bind();
	glViewport(0, 0, iWidth, iHeight);
	
	glClearColor(1, 0, 0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	

	m_Shader->Use();
	
	m_Shader->SetUniform("screen", m_WinSize);
	m_Shader->SetUniform("scale", 1.0f);
	glActiveTexture(GL_TEXTURE0);
	m_Shader->SetUniform("Texture", 0);
	glBindTexture(GL_TEXTURE_2D, iIndex);
	glBindVertexArray(m_Quad.VAO);
	glDrawArrays(m_Quad.Topology, 0, 4);
	fb.UnBind();
	return fb.iIndex;
}

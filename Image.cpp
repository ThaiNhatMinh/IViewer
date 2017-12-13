#include "stdafx.h"
#include "Image.h"
#include <FreeImage.h>
Image::Image(vec2 windowsize,Shader* p):m_Shader(p)
{
	//m_Shader = std::unique_ptr<Shader>(new Shader("Data\\Shader\\Quad.vs", "Data\\Shader\\Quad.fs"));
	
	m_WinSize = windowsize;
	m_Scale = 1.0f;
	m_iCurrentAction = -1;

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

	if (bpp == 24) iBpp = GL_RGB;
	
	else if (bpp == 32) iBpp = GL_RGBA;
	iType = GL_RGB;
	
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
	

	ScreenMesh newMesh;
	newMesh.Finalize(m_Shader);
	newMesh.Resize(iWidth, iHeight);
	newMesh.W = iWidth;
	newMesh.H = iHeight;
	

	m_CurrentMesh = newMesh;
	
	iIndex = id;
	m_Path = file;

	FreeImage_Unload(img_bm);


}

FREE_IMAGE_FORMAT ExtToFreeExt(const char* ext)
{
	if (!strcmp(ext, ".jpg")) return FREE_IMAGE_FORMAT::FIF_JPEG;
	else if(!strcmp(ext, ".bmp"))return FREE_IMAGE_FORMAT::FIF_BMP;
	//else if (!strcmp(ext, ".dds"))return FREE_IMAGE_FORMAT::FIF_DDS;
	else if (!strcmp(ext, ".ico"))return FREE_IMAGE_FORMAT::FIF_ICO;
	else if (!strcmp(ext, ".png"))return FREE_IMAGE_FORMAT::FIF_PNG;
	//else if (!strcmp(ext, ".psd"))return FREE_IMAGE_FORMAT::FIF_PSD;
	else if (!strcmp(ext, ".tga"))return FREE_IMAGE_FORMAT::FIF_TARGA;
	else if (!strcmp(ext, ".bmp"))return FREE_IMAGE_FORMAT::FIF_BMP;

}


bool Image::SaveFile(const char * file)
{
	ScreenMesh mesh = m_CurrentMesh;
	mesh.Resize(mesh.W, mesh.H);
	FrameBuffer fb(mesh.W, mesh.H, iBpp, iType);
	
	
	fb.Bind();
	glViewport(0, 0, mesh.W, mesh.H);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1.0f);


	vec2 size(mesh.W, mesh.H);
	m_Shader->Use();

	m_Shader->SetUniform("Texture", 0);
	mat4 m = glm::ortho(-size.x / 2, size.x / 2, -size.y / 2, size.y / 2);
	m_Shader->SetUniformMatrix("Proj", glm::value_ptr(m));
	glBindTexture(GL_TEXTURE_2D, iIndex);
	glBindVertexArray(mesh.VAO);
	glDrawArrays(mesh.Topology, 0, 4);
	

	GLint bpp;
	if (iBpp == GL_RGB) bpp = 24;
	else if (iBpp == GL_RGBA) bpp = 32;
	
	
	FIBITMAP* img_bm = FreeImage_Allocate(mesh.W, mesh.H, bpp);
	int* pixels = (int*)FreeImage_GetBits(img_bm);


	glReadBuffer(GL_COLOR_ATTACHMENT0);
	
	glReadPixels(0, 0, mesh.W, mesh.H, iBpp, GL_UNSIGNED_BYTE, pixels);

	if (FreeImage_Save(FREE_IMAGE_FORMAT::FIF_JPEG, img_bm, file))printf("Info: Save success !!");
	else printf("Info: Save fail !!");

	// Clean up
	FreeImage_Unload(img_bm);	
	fb.UnBind();

	
	return true;
}


Image::~Image()
{
	

}

GLuint Image::Render()
{

	//FrameBuffer& fb = m_List.back();
	
	//fb.Bind();
	//glViewport(0, 0, iWidth, iHeight);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor(1, 1, 1, 1.0f);
	
	

	//m_Shader->Use();
	
	//vec2 size(iWidth, iHeight);
	//mat4 m;// = glm::ortho(-size.x / 2, size.x / 2, -size.y / 2, size.y / 2);
	//m_Shader->SetUniformMatrix("Proj", glm::value_ptr(m));

	//m_Shader->SetUniform("Texture", 0);
	m_CurrentMesh.Resize(m_Scale*iWidth, m_Scale*iHeight);

	for (size_t i = 0; i < m_ActionList.size(); i++)
		m_ActionList[i]->Do();
	glBindTexture(GL_TEXTURE_2D, iIndex);
	glBindVertexArray(m_CurrentMesh.VAO);
	glDrawArrays(m_CurrentMesh.Topology, 0, 4);
	
	
	//fb.UnBind();
	return iIndex;
}

void Image::ApplyScale()
{
	
	
	int newiWidth = iWidth*m_Scale;
	int newiHeight = iHeight*m_Scale;

	// Check if anything change
	if ((newiWidth- iWidth)<10 && (newiHeight- iHeight)<10) return;

	ResizeAction* pAc = new ResizeAction(this, vec2(newiWidth, newiHeight), vec2(iWidth, iHeight));

	m_ActionList.push_back(std::unique_ptr<IAction>(pAc));
	m_iCurrentAction++;

	

}

void Image::Undo()
{
	if (m_iCurrentAction != -1 && m_iCurrentAction > 0)
	{
		m_iCurrentAction--;

		m_ActionList[m_iCurrentAction]->Undo();
	}
	
}

void Image::Redo()
{
	if (m_iCurrentAction < m_ActionList.size() - 1)
	{
		m_iCurrentAction++;
		m_ActionList[m_iCurrentAction]->Redo();
	}
}

bool Image::CanUndo()
{
	if (m_iCurrentAction == -1) return false;
	return true;
}

bool Image::CanRedo()
{
	if (m_iCurrentAction == -1) return false;
	return true;
}

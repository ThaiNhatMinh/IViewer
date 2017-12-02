#pragma once
#include "stdafx.h"

struct imguiVertex
{
	vec2 pos;
	vec2 uv;
	vec4 color;
};

class imguiMesh 
{
public:
	GLuint VAO, VBO, EBO;
	GLenum	Topology;
	vector<imguiVertex>	m_Vertexs;
	vector<unsigned int>	m_Indices;
public:
	imguiMesh();
	~imguiMesh();

	// dirive class can redifine this function to add more data;
	virtual void Finalize(Shader* p);
};

struct DefaultVertex
{
	vec2 pos;
	vec2 uv;
};

class Mesh 
{
public:
	GLuint VAO, VBO;
	GLenum	Topology;
	vector<DefaultVertex>	m_Vertexs;
public:
	Mesh();
	~Mesh();

	// dirive class can redifine this function to add more data;
	void Finalize(Shader* p);
	void Resize(float w, float h);
	
};
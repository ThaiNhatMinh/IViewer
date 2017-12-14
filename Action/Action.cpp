#include "stdafx.h"
#include "Action.h"

ResizeAction::ResizeAction(Image* p, vec2 newsize, vec2 oldSize):IAction(p),m_newSize(newsize),m_Oldsize(oldSize)
{
}

void ResizeAction::Undo()
{
	int newiWidth = m_Oldsize.x;
	int newiHeight = m_Oldsize.y;

	m_Owner->m_CurrentMesh.Resize(newiWidth, newiHeight);



	m_Owner->iWidth = m_Oldsize.x;
	m_Owner->iHeight = m_Oldsize.y;
	m_Owner->m_Scale = 1;
}

void ResizeAction::Redo()
{
	int newiWidth = m_newSize.x;
	int newiHeight = m_newSize.y;

	m_Owner->m_CurrentMesh.Resize(newiWidth, newiHeight);
	


	m_Owner->iWidth = m_newSize.x;
	m_Owner->iHeight = m_newSize.y;
	m_Owner->m_Scale = 1;
}

RotateAction::RotateAction(Image * p, float _angle):IAction(p),angle(_angle)
{
}

void RotateAction::Undo()
{
	float rads = glm::radians(-angle);
	mat4 rotate = glm::rotate(mat4(), rads, vec3(0, 0, 1));
	ScreenMesh& m = m_Owner->m_CurrentMesh;

	for (int i = 0; i < 4; i++)
	{
		m.m_Vertexs[i].pos = rotate*vec4(m.m_Vertexs[i].pos, 0.0f, 1.0f);
		m.m_Vertexs[i].uv = rotate*vec4(m.m_Vertexs[i].uv, 0.0f, 1.0f);
	}
	m.UpdateBuffer();
	int newsizeW = m.m_Vertexs[0].pos.x - m.m_Vertexs[1].pos.x;
	int newsizeH = m.m_Vertexs[0].pos.y + m.m_Vertexs[1].pos.y;
	newsizeW = newsizeW > 0 ? newsizeW : -newsizeW;
	newsizeH = newsizeH > 0 ? newsizeH : -newsizeH;
	m.W = newsizeW;
	m.H = newsizeH;
	m_Owner->iWidth = newsizeW;
	m_Owner->iHeight = newsizeH;
}

void RotateAction::Redo()
{
	float rads = glm::radians(angle);
	mat4 rotate = glm::rotate(mat4(), rads, vec3(0, 0, 1.0f));
	ScreenMesh& m = m_Owner->m_CurrentMesh;

	//std::cout << m.W << " " << m.H << std::endl;
	for (int i = 0; i < 4; i++)
	{
		m.m_Vertexs[i].pos = rotate*vec4(m.m_Vertexs[i].pos,0.0f,1.0f);
		m.m_Vertexs[i].uv = rotate*vec4(m.m_Vertexs[i].uv, 0.0f, 1.0f);
	}
	m.UpdateBuffer();
	int newsizeW = m.m_Vertexs[0].pos.x - m.m_Vertexs[1].pos.x;
	int newsizeH = m.m_Vertexs[0].pos.y + m.m_Vertexs[1].pos.y;
	newsizeW = newsizeW > 0 ? newsizeW : -newsizeW;
	newsizeH = newsizeH > 0 ? newsizeH : -newsizeH;
	m.W = newsizeW;
	m.H = newsizeH;
	m_Owner->iWidth = newsizeW;
	m_Owner->iHeight = newsizeH;
	
}

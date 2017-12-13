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

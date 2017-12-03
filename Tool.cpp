#include "stdafx.h"
#include "Tool.h"


Tool::Tool(const char* icon)
{
	m_Icon = new char[4];
	memcpy(m_Icon, icon, 4);
}


Tool::~Tool()
{
	delete[] m_Icon;
}

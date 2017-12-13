#pragma once

// Action interface
class IAction
{
protected:
	Image* m_Owner;
public:
	IAction(Image* p) :m_Owner(p) {};
	virtual void Do() {};
	virtual void Undo() = 0;
	virtual void Redo() = 0;
	virtual ~IAction() {};
};


class ResizeAction:public IAction
{
private:
	vec2 m_newSize, m_Oldsize;
public:
	ResizeAction(Image* p,vec2 newsize,vec2 oldSize);
	virtual void Undo();
	virtual void Redo();
};
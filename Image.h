#pragma once
enum ActionType
{
	AC_DEFAULT, //init action
	AC_RESIZE,
	AC_ROTATE,
	AC_MAX
};

struct ImageState
{
	ScreenMesh mesh;
	ActionType type;
	void* buffer;
	// if scale is float(4 byte)
	// if rotate is angle float(4 byte)
};
class Image
{
	friend class IAction;
public:
	Image(vec2 windowsize, Shader* p);
	void LoadTexture(const char* file);
	bool SaveFile(const char* file);
	~Image();

	//vector<FrameBuffer> m_List;
	vector<std::unique_ptr<IAction>> m_ActionList;
	GLuint iIndex; // Texture index
	GLuint iWidth; // 
	GLuint iHeight;
	GLuint iType, iBpp;
	float m_Scale;
	ScreenMesh m_CurrentMesh;

	GLuint Render();
	float &GetScale() { return m_Scale; }
	vec2& GetSize() { return vec2(iWidth, iHeight)*m_Scale; };

	void ApplyScale();
	void Undo();
	void Redo();
	bool CanUndo();
	bool CanRedo();
	void ApplyAction(IAction*);
	string GetName() { return name; }
private:

	int m_bJustRedo;
	Shader* m_Shader;
	string name;
	string m_Path;
	char m_Ext[5];
	vec2 m_WinSize;
	
	int m_iCurrentAction;

};


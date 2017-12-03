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
public:
	Image(vec2 windowsize, Shader* p);
	void LoadTexture(const char* file);
	bool SaveFile(const char* file);
	~Image();

	//vector<FrameBuffer> m_List;
	vector<ImageState> m_List;
	GLuint iIndex; // Texture index
	GLuint iWidth; // 
	GLuint iHeight;
	GLuint iType, iBpp;

	GLuint Render();
	float &GetScale() { return m_Scale; }
	vec2& GetSize() { return vec2(iWidth, iHeight)*m_Scale; };

	void ApplyScale();
	void Undo();
	void Redo();
private:
	Shader* m_Shader;
	ScreenMesh m_CurrentMesh;
	string m_Path;
	char m_Ext[5];
	vec2 m_WinSize;
	float m_Scale;
	int frameindex;

};


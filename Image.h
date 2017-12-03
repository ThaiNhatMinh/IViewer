#pragma once
class Image
{
public:
	Image(vec2 windowsize);
	void LoadTexture(const char* file);
	bool SaveFile(const char* file);
	~Image();

	vector<FrameBuffer> m_List;

	GLuint iIndex; // Texture index
	GLuint iWidth; // 
	GLuint iHeight;
	GLuint iType, iBpp;
	bool hasload = 0;
	bool HasTexture();
	GLuint Render();
	float &GetScale() { return m_Scale; }
	vec2& GetSize() { return vec2(iWidth, iHeight)*m_Scale; };
private:
	std::unique_ptr<Shader> m_Shader;
	ScreenMesh m_Quad;
	string m_Path;
	vec2 m_WinSize;
	float m_Scale;

};


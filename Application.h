#pragma once
enum DialogEnum
{
	D_STYLE_EDITOR,
	D_BG_COLOR,
	D_MAX_ENUM
};

struct EXTData
{
	char Format[512];
	int NumFormat;
};
class Application
{
public:
	Application();
	~Application();
	void RunMainLoop();

protected:
	void OpenFile();
	void CloseFile() {};
	void SaveFile();
	void SaveFileAs();

	void RenderUI();
	void RenderImage();
	std::unique_ptr<Windows> m_Window;
	std::unique_ptr<OpenGLRenderer> m_Renderer;
	std::unique_ptr<SystemUI> m_UI;
	std::unique_ptr<Image> m_CurrentImage;

	ScreenMesh m_Screen;
	Shader m_Shader;
	bool DialogOption[D_MAX_ENUM];
	EXTData m_SaveExt;
	EXTData m_OpenExt;
};


#pragma once
enum DialogEnum
{
	D_STYLE_EDITOR,
	D_BG_COLOR,
	D_BG_TOOLS,
	D_RESIZE,
	D_ROTATE_LEFT,
	D_ROTATE_RIGHT,
	D_IMAGE_LIST,
	D_MAX_ENUM
};

struct EXTData
{
	char Format[512];
	int NumFormat;
};
class Application
{
	friend class Tool;
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
	std::vector<std::unique_ptr<Image>> m_CurrentImage;
	int	m_iActiveImage;
	ScreenMesh m_Screen;
	Shader m_Shader;
	bool DialogOption[D_MAX_ENUM];
	vector<std::unique_ptr<Tool>> m_Tools;
	EXTData m_SaveExt;
	EXTData m_OpenExt;
};


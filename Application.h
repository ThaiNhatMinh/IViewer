#pragma once
class Application
{
public:
	Application();
	~Application();
	void RunMainLoop();

protected:
	void OpenFile();
	void CloseFile() {};
	void SaveFile() {};
	void SaveFileAs() {};
	void RenderUI();
	std::unique_ptr<Windows> m_Window;
	std::unique_ptr<OpenGLRenderer> m_Renderer;
	std::unique_ptr<SystemUI> m_UI;
	std::unique_ptr<Image> m_CurrentImage;
	
};


#include "stdafx.h"
#include "Application.h"
#include <IL\il.h>

Application::Application()
{
	ilInit();
	m_Window = std::unique_ptr<Windows>(new Windows());
	m_Renderer = std::unique_ptr<OpenGLRenderer>(new OpenGLRenderer(m_Window.get()));
	m_UI = std::unique_ptr<SystemUI>(new SystemUI(m_Window.get()));


	m_CurrentImage = std::unique_ptr<Image>(new Image);

	glDisable(GL_CULL_FACE);
}


Application::~Application()
{
	ilShutDown();
}

void Application::RunMainLoop()
{
	m_Window->ShowWindows();

	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!m_Window->ShouldClose())
	{
		glfwPollEvents();
		
		m_Renderer->Clear();

		if (m_CurrentImage->HasTexture()) 
			m_CurrentImage->Render();

		RenderUI();

		
	
		m_Renderer->SwapBuffer();
	}
}

void Application::OpenFile()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	HANDLE hf;              // file handle

							// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_Window->GetHandle();
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = ".jpg\0*.jpg\0*.bmp\0*.bmp\0*.tga\0*.tga";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);
	cout << ofn.lpstrFile << endl;
	m_CurrentImage->LoadTexture(ofn.lpstrFile);
}

void Application::RenderUI()
{

	m_UI->NewFrame();

	ImGui::PushFont(m_UI->GetCurrentFont());
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "CTRL+N")) {};
			if (ImGui::MenuItem("Open", "CTRL+O")) OpenFile();
			if (ImGui::MenuItem("Save", "CTRL+S")) {}
			if (ImGui::MenuItem("Save As..", "CTRL+SHIFT+Z")) {}
			if (ImGui::MenuItem("Close")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Quit")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About IMaditor")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGui::PopFont();
	ImGui::Render();
}

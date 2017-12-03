#include "stdafx.h"
#include "Application.h"


Application::Application()
{
	
	m_Window = std::unique_ptr<Windows>(new Windows());
	m_Renderer = std::unique_ptr<OpenGLRenderer>(new OpenGLRenderer(m_Window.get()));
	m_UI = std::unique_ptr<SystemUI>(new SystemUI(m_Window.get()));


	m_CurrentImage = std::unique_ptr<Image>(new Image(m_Window->GetWindowSize()));

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	m_Shader.Load("Data\\Shader\\Screen.vs", "Data\\Shader\\Screen.fs");
	m_Screen.Finalize(&m_Shader);	
	
	const char* A = "Jpeg\0*.JPG\0Windows Bitmap\0*.BMP\0Truevision Targa\0*.TGA\0Adobe PhotoShop\0*.psd\0DirectDraw Surface\0*.DDS\0";
	memcpy(m_OpenExt.Format, A, 103);
	m_OpenExt.NumFormat = 5;
	const char* B= "Jpeg\0*.JPG\0Windows Bitmap\0*.BMP\0Truevision Targa\0*.TGA\0DirectDraw Surface\0*.DDS\0";
	memcpy(m_SaveExt.Format, B, 81);
	m_SaveExt.NumFormat = 4;

}


Application::~Application()
{
	
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

		RenderUI();

		RenderImage();

		

		ImGui::Render();
	
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
	ofn.lpstrFilter = m_OpenExt.Format;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
		//cout << ofn.lpstrFile << endl;
	{
		m_CurrentImage->LoadTexture(ofn.lpstrFile);
		//m_Screen.Resize(m_CurrentImage->GetSize().x, m_CurrentImage->GetSize().y);
	}
}

void Application::SaveFile()
{
	
}

void Application::SaveFileAs()
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
	ofn.lpstrFilter = m_SaveExt.Format;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = "jpg";
	if (GetSaveFileName(&ofn))
	{
		//cout << ofn.nFilterIndex << endl;
		/*int i = 1;
		int pos = 0;
		char num = 0;
		//const char* B = "Jpeg\0*.JPG\0Windows Bitmap\0*.BMP\0Truevision Targa\0*.TGA\0DirectDraw Surface\0*.DDS\0";
		char ext[5];
		ext[4]='\0';
		while (num<m_SaveExt.NumFormat)
		{
			if (i == ofn.nFilterIndex)
			{
				pos++;
				while (m_SaveExt.Format[pos] != '\0') pos++;
				memcpy(ext, &m_SaveExt.Format[pos+2], 4);
				break;
			}
			int count = 0;
			while (count != 2)
			{
				if (m_SaveExt.Format[pos++] == '\0') count++;
			}
			pos--;
			num++;
			i++;
		}
		char path[512];
		sprintf(path, "%s%s", ofn.lpstrFile, ext);*/
		m_CurrentImage->SaveFile(ofn.lpstrFile);
	}
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
			if (ImGui::MenuItem("Save", "CTRL+S")) SaveFile();
			if (ImGui::MenuItem("Save As..", "CTRL+SHIFT+Z")) SaveFileAs();
			if (ImGui::MenuItem("Close")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Quit")) m_Window->Close();
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
		if (ImGui::BeginMenu("Option"))
		{
			if (ImGui::MenuItem("BackGround Color")) DialogOption[D_BG_COLOR] = 1;
			if (ImGui::MenuItem("Style Editor")) DialogOption[D_STYLE_EDITOR] = 1;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About IViewer")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (DialogOption[D_BG_COLOR])
	{
		ImGui::Begin("Background Color", &DialogOption[D_BG_COLOR]);
		ImGui::ColorEdit3("clear color", (float*)&m_Renderer->GetClearColor());
		ImGui::End();
	}
	if (DialogOption[D_STYLE_EDITOR])
	{
		ImGui::Begin("Style Editor", &DialogOption[D_STYLE_EDITOR]); 
		ImGui::ShowStyleEditor(); 
		ImGui::End();
	}

	ImGui::Begin("Style Editor", nullptr);
	ImGui::SliderFloat("Slide", &m_CurrentImage->GetScale(), 0.1f, 2.0f);
	ImGui::End();

	ImGui::PopFont();
}

void Application::RenderImage()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 1. Render image to texture
	GLuint index;
	if (m_CurrentImage->HasTexture())
	{
		
		index = m_CurrentImage->Render();
		m_Screen.Resize(m_CurrentImage->GetSize().x, m_CurrentImage->GetSize().y);

		// 2. Render to screen
		vec2 size = m_Window->GetWindowSize();
		glViewport(0, 0, size.x, size.y);
		m_Renderer->Clear();
		glClearColor(0.1, 0.1, 0.1, 1.0f);
		m_Shader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, index);
		m_Shader.SetUniform("Texture", 0);
		mat4 m = glm::ortho(-size.x/2, size.x/2, -size.y/2, size.y/2);
		//m = m*glm::translate(glm::mat4(), vec3(size.x / 2, size.y / 2, 0));
		m_Shader.SetUniformMatrix("Proj", glm::value_ptr(m));
		
		glBindVertexArray(m_Screen.VAO);
		glDrawArrays(m_Screen.Topology, 0, 4);
	}

}

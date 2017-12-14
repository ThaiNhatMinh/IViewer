#include "stdafx.h"
#include "Application.h"


Application::Application()
{
	
	m_Window = std::unique_ptr<Windows>(new Windows());
	m_Renderer = std::unique_ptr<OpenGLRenderer>(new OpenGLRenderer(m_Window.get()));
	m_UI = std::unique_ptr<SystemUI>(new SystemUI(m_Window.get()));


	m_CurrentImage = nullptr;

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	m_Shader.Load("Data\\Shader\\Screen.vs", "Data\\Shader\\Screen.fs");
	m_Screen.Finalize(&m_Shader);	
	
	const char* A = "Jpeg\0*.JPG\0Windows Bitmap\0*.BMP\0Truevision Targa\0*.TGA\0Adobe PhotoShop\0*.psd\0DirectDraw Surface\0*.DDS\0";
	memcpy(m_OpenExt.Format, A, 103);
	m_OpenExt.NumFormat = 5;
	const char* B= "Jpeg\0*.jpg\0Windows Bitmap\0*.bmp\0Icon\0*.ico\0Portable Network Graphics\0*.png\0Truevision Targa\0*.tga\0";
	memcpy(m_SaveExt.Format, B, 81);
	m_SaveExt.NumFormat = 4;

	Tool* OpenFile = new Tool("\xEF\x84\x95");
	OpenFile->Action = [](Application* p)
	{
		p->OpenFile();
	};
	m_Tools.push_back(std::unique_ptr<Tool>(OpenFile)); //open

	Tool* CloseFile = new Tool("\xEF\x80\x8D");
	CloseFile->Action = [](Application* p)
	{
		Image* pI = p->m_CurrentImage.get();
		if (pI)
		{
			 p->m_CurrentImage.release();
			delete pI;
		}
	};
	m_Tools.push_back(std::unique_ptr<Tool>(CloseFile));

	Tool* SaveFile = new Tool("\xEF\x83\x87");
	SaveFile->Action = [](Application* p)
	{
		
		if (p->m_CurrentImage)
		{
			p->SaveFile();
		}
	};

	//m_Tools.push_back(Tool("")); //save
	//m_Tools.push_back(Tool("\xEF\x81\x84")); //edit\xEF\x80\x90
	Tool* Undo = new Tool("\xEF\x83\xA2");
	Undo->Action = [](Application* p)
	{
		Image* pI = p->m_CurrentImage.get();
		if (pI) pI->Undo();
	};
	
	m_Tools.push_back(std::unique_ptr<Tool>(Undo));

	Tool* Redo = new Tool("\xEF\x80\x9E");
	Redo->Action = [](Application* p)
	{
		Image* pI = p->m_CurrentImage.get();
		if (pI) pI->Redo();
	};

	m_Tools.push_back(std::unique_ptr<Tool>(Redo));

	Tool* ZoomIm = new Tool("\xEF\x80\x8E");
	ZoomIm->Action = [](Application* p)
	{
		Image* pI = p->m_CurrentImage.get();
		if (pI) pI->GetScale() += 0.1f;
	};

	m_Tools.push_back(std::unique_ptr<Tool>(ZoomIm));

	Tool* ZoomOut = new Tool("\xEF\x80\x90");
	ZoomOut->Action = [](Application* p)
	{
		Image* pI = p->m_CurrentImage.get();
		if (pI) pI->GetScale() -= 0.1f;
	};

	m_Tools.push_back(std::unique_ptr<Tool>(ZoomOut));

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
		m_CurrentImage = std::unique_ptr<Image>(new Image(m_Window->GetWindowSize(),&m_Shader));
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

const int menubarHeight = 22;
const int optionsbarHeight = 45;
void Application::RenderUI()
{

	m_UI->NewFrame();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2,0.3,0.4,0.5));
		{
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
					bool canUndo = m_CurrentImage!=nullptr ? (m_CurrentImage->CanUndo() ? true : false) : false;
					bool canRedo = m_CurrentImage != nullptr ? (m_CurrentImage->CanRedo() ? true : false) : false;

					if (ImGui::MenuItem("Undo", "CTRL+Z",false, canUndo)) m_CurrentImage->Undo();
					if (ImGui::MenuItem("Redo", "CTRL+Y", false, canRedo)) m_CurrentImage->Redo();  // Disabled item
					ImGui::Separator();
					if (ImGui::MenuItem("Cut", "CTRL+X")) {}
					if (ImGui::MenuItem("Copy", "CTRL+C")) {}
					if (ImGui::MenuItem("Paste", "CTRL+V")) {}
					ImGui::Separator();
					if (ImGui::MenuItem("Resize",nullptr, false, (m_CurrentImage != nullptr))) DialogOption[D_RESIZE] = 1;
					if (ImGui::MenuItem("Rotate left", nullptr, false, (m_CurrentImage != nullptr)))
					{
						RotateAction* Rotate = new RotateAction(m_CurrentImage.get(), -90.0f);
						m_CurrentImage->ApplyAction(Rotate);

					}
					if (ImGui::MenuItem("Rotate right", nullptr, false, (m_CurrentImage != nullptr)))
					{
						RotateAction* Rotate = new RotateAction(m_CurrentImage.get(), 90.0f);
						m_CurrentImage->ApplyAction(Rotate);

					}
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
			if (DialogOption[D_RESIZE])
			{
				ImGui::Begin("Resize", &DialogOption[D_RESIZE]);
				ImGui::SetWindowSize(ImVec2(300, 150));
				ImGui::Text("Size: %dx%d", (int)m_CurrentImage->GetSize().x, (int)m_CurrentImage->GetSize().y);
				ImGui::SliderFloat("reiszeImage", &m_CurrentImage->GetScale(), 0.1f, 3.f);
				int a = 1;
				ImGui::Combo("Percent", &a, " 25%\0 50%\0 75%\0 200%\0");
				if (ImGui::Button("Apply", ImVec2(40, 25)))
				{
					m_CurrentImage->ApplyScale();
				}
				ImGui::End();
			}
			

			ImGui::PopFont();
		}
		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.20f, 0.20f, 0.47f, 0.60f));
		{
			//ImGui::PushFont(m_UI->GetSysbolFont());
			ImGui::Begin("optionsBar", &DialogOption[D_BG_TOOLS], ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
			{
				ImGui::SetWindowPos(ImVec2(0, menubarHeight));
				ImGui::SetWindowSize(ImVec2(40, m_Window->GetWindowSize().y));

				for (int i = 0; i < m_Tools.size(); i++)
				{
					ImGui::PushID(i);
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
					if (ImGui::Button(m_Tools[i]->Icon(), ImVec2(30, 30))) m_Tools[i]->Action(this);
					ImGui::PopStyleColor(1);
					ImGui::PopID();
				}
			}
			ImGui::End();
			//ImGui::PopFont();
		}
		ImGui::PopStyleColor();
	}
	ImGui::PopStyleVar();
}

void Application::RenderImage()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLuint index;
	if (m_CurrentImage!=nullptr)
	{
		
		//index = m_CurrentImage->Render();
		//m_Screen.Resize(m_CurrentImage->GetSize().x, m_CurrentImage->GetSize().y);

		// 2. Render to screen
		vec2 size = m_Window->GetWindowSize();
		//glViewport(0, 0, size.x, size.y);
		//m_Renderer->Clear();
		//glClearColor(0.1, 0.1, 0.1, 1.0f);

		m_Shader.Use();
		m_Shader.SetUniform("Texture", 0);
		mat4 m = glm::ortho(-size.x / 2, size.x / 2, -size.y / 2, size.y / 2);
		m_Shader.SetUniformMatrix("Proj", glm::value_ptr(m));

		index = m_CurrentImage->Render();

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, index);
		
		//m = m*glm::translate(glm::mat4(), vec3(size.x / 2, size.y / 2, 0));
		
		
		//glBindVertexArray(m_Screen.VAO);
		//glDrawArrays(m_Screen.Topology, 0, 4);
	}

}

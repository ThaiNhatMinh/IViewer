#pragma once
class Image
{
public:
	Image();
	void LoadTexture(const char* file);
	~Image();

	vector<FrameBuffer> m_List;
	Texture m_Tex;
	bool HasTexture();
	void Render();
private:
	std::unique_ptr<Shader> m_Shader;
	Mesh m_Quad;

};


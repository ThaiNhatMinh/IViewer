#pragma once
class Application;
class Tool
{
private:
	char* m_Icon;
	bool Status;
	
public:
	std::function<void(Application* p)> Action;
	Tool(const char* icon);
	~Tool();

	Tool& operator=(const Tool& other)
	{
		memcpy(m_Icon, other.m_Icon, 4);
	}
	Tool(const Tool& other)
	{
		//if (m_Icon) delete[] m_Icon;
		m_Icon = new char[4];
		memcpy(m_Icon, other.m_Icon, 4);
	}

	const char* Icon() {
		return m_Icon;
	};
	bool& GetStatus() { return Status; };
};


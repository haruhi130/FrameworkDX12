#pragma once

#include "../System/Window/Window.h"

class Application
{
public:
	// ������
	bool Init(int width = 1280,int height = 720);

	// �Q�[�����s
	void Execute();

private:
	Window m_window;

	Application(){}
	~Application(){}
public:
	static Application& GetInstance()
	{
		static Application instance;
		return instance;
	}
};
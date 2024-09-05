#pragma once

#include "../System/Window/Window.h"

class Application
{
public:
	// �Q�[��������
	bool Init(int width = 1280,int height = 720);

	// �Q�[�����s
	void Execute();

	// �Q�[���I��
	void Terminate();

private:

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();

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
#pragma once

#include "../System/Window/Window.h"

class Application
{
public:
	// �Q�[��������
	bool Init(int width = window_width,int height = window_height);

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
	void PostDraw();
	void DrawSprite();

	void ImGuiUpdate();

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
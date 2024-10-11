#pragma once

#include "../System/Window/Window.h"

class Application
{
public:
	// ゲーム初期化
	bool Init(int width = windowWidth,int height = windowHeight);

	// ゲーム実行
	void Execute();

	// ゲーム終了
	void Terminate();

private:

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
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
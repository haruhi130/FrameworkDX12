#pragma once

#include "../System/Window/Window.h"

class Application
{
public:
	// ゲーム初期化
	bool Init(int width = 1280,int height = 720);

	// ゲーム実行
	void Execute();

	// ゲーム終了
	void Terminate();

	Shader& GetShader()const { return *m_spShader; }

private:

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();

	Window m_window;

	std::shared_ptr<Shader> m_spShader = nullptr;

	Application(){}
	~Application(){}
public:
	static Application& GetInstance()
	{
		static Application instance;
		return instance;
	}
};
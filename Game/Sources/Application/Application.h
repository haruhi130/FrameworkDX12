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

private:
	// DLLディレクトリのセットとロード
	void SetDirectoryAndLoadDll();

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
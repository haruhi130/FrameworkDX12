#pragma once

#include "../System/Window/Window.h"

class Application
{
public:
	// ‰Šú‰»
	bool Init(int width = 1280,int height = 720);

	// ƒQ[ƒ€Às
	void Execute();

	void Terminate();

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
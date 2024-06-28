#pragma once

#include "../System/Window/Window.h"

class Application
{
public:
	// ÉQÅ[ÉÄé¿çsópä÷êî
	void Execute();

private:
	Window m_window;

	Application(){}
public:
	static Application& GetInstance()
	{
		static Application instance;
		return instance;
	}
};
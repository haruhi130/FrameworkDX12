#include "./Application/Application.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE,LPSTR, int)
{
	Application::GetInstance().Execute();
	return 0;
}
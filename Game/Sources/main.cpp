#include "./Application/Application.h"

int WINAPI WinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE,_In_ LPSTR,_In_ int)
{
	// アプリケーション実行
	Application::GetInstance().Execute();
	
	// アプリケーション終了
	Application::GetInstance().Terminate();
	
	return 0;
}
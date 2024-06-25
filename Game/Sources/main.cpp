#include "./Application/Application.h"

int WINAPI WinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE,_In_ LPSTR,_In_ int)
{
	// メモリリーク検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// アプリケーション実行
	Application::GetInstance().Execute();
	
	// COM解放
	CoUninitialize();
	
	return 0;
}
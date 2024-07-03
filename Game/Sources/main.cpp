#include "./Application/Application.h"

int WINAPI WinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE,_In_ LPSTR,_In_ int)
{
	// ���������[�N���m
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM������
	HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	// �A�v���P�[�V�������s
	Application::GetInstance().Execute();
	
	Application::GetInstance().Terminate();
	
	return 0;
}
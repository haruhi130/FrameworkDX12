#include "./Application/Application.h"

int WINAPI WinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE,_In_ LPSTR,_In_ int)
{
	// �A�v���P�[�V�������s
	Application::GetInstance().Execute();
	
	// �A�v���P�[�V�����I��
	Application::GetInstance().Terminate();
	
	return 0;
}
#include "Application.h"

void Application::Execute()
{
	// ウィンドウ作成
	if (!m_window.Create(1280, 720, L"FrameworkDX12", L"Window"))
	{
		assert(0 && "ウィンドウ作成失敗");
		return;
	}

	// ゲームループ
	while (true)
	{
		if (!m_window.ProcessMessage())
		{
			break;
		}
	}
}

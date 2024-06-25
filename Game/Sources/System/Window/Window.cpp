#include "Window.h"

LRESULT WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// メッセージ毎に処理を選択
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

bool Window::Create(int width, int height, const std::wstring& titleName, const std::wstring& windowClassName)
{
	HINSTANCE hInst = GetModuleHandle(0);

	//===========================================
	// メインウィンドウ作成
	//===========================================

	// ウィンドウ定義
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;
	wc.lpszClassName = windowClassName.c_str();
	wc.hInstance = hInst;

	// ウィンドウ登録
	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	// ウィンドウ作成
	m_hWnd = CreateWindow(
		windowClassName.c_str(),
		titleName.c_str(),
		WS_OVERLAPPEDWINDOW - WS_THICKFRAME,
		0,
		0,
		width,
		height,
		nullptr,
		nullptr,
		hInst,
		this
	);

	if (m_hWnd == nullptr)
	{
		return false;
	}

	// ウィンドウ表示
	ShowWindow(m_hWnd, SW_SHOW);

	// ウィンドウ更新
	UpdateWindow(m_hWnd);

	return true;
}

bool Window::ProcessMessage()
{
	// メッセージ取得
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_DESTROY)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

#include "Window.h"

LRESULT WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// メッセージ毎に処理を選択
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0); // OSへ終了を伝える
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

bool Window::Create(int windowWidth, int windowHeight, const std::wstring& titleName, const std::wstring& windowClassName)
{
	//===========================================
	// メインウィンドウ作成
	//===========================================

	HINSTANCE hInst = GetModuleHandle(0);

	// ウィンドウ定義
	m_wndClass.cbSize = sizeof(WNDCLASSEX);
	m_wndClass.lpfnWndProc = (WNDPROC)WindowProcedure;
	m_wndClass.lpszClassName = windowClassName.c_str();
	m_wndClass.hInstance = hInst;

	// ウィンドウ登録
	if (!RegisterClassEx(&m_wndClass))
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
		windowWidth,
		windowHeight,
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
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void Window::Terminate() const
{
	// ウィンドウ登録解除
	UnregisterClass(m_wndClass.lpszClassName, m_wndClass.hInstance);
}

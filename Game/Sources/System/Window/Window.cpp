#include "Window.h"

LRESULT WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ���b�Z�[�W���ɏ�����I��
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
	// ���C���E�B���h�E�쐬
	//===========================================

	// �E�B���h�E��`
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;
	wc.lpszClassName = windowClassName.c_str();
	wc.hInstance = hInst;

	// �E�B���h�E�o�^
	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	// �E�B���h�E�쐬
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

	// �E�B���h�E�\��
	ShowWindow(m_hWnd, SW_SHOW);

	// �E�B���h�E�X�V
	UpdateWindow(m_hWnd);

	return true;
}

bool Window::ProcessMessage()
{
	// ���b�Z�[�W�擾
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

#include "Window.h"

LRESULT WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ���b�Z�[�W���ɏ�����I��
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0); // OS�֏I����`����
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
	// ���C���E�B���h�E�쐬
	//===========================================

	HINSTANCE hInst = GetModuleHandle(0);

	// �E�B���h�E��`
	m_wndClass.cbSize = sizeof(WNDCLASSEX);
	m_wndClass.lpfnWndProc = (WNDPROC)WindowProcedure;
	m_wndClass.lpszClassName = windowClassName.c_str();
	m_wndClass.hInstance = hInst;

	// �E�B���h�E�o�^
	if (!RegisterClassEx(&m_wndClass))
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
	// �E�B���h�E�o�^����
	UnregisterClass(m_wndClass.lpszClassName, m_wndClass.hInstance);
}

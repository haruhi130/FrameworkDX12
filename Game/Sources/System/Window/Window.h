#pragma once

class Window
{
public:
	// �E�B���h�E�쐬
	bool Create(int windowWidth, int windowHeight, const std::wstring& titleName, const std::wstring& windowClassName);

	// �E�B���h�E�n���h���擾
	inline HWND GetWndHandle()const { return m_hWnd; }

	bool ProcessMessage();

private:
	HWND m_hWnd = {};
};
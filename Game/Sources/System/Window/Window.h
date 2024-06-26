#pragma once

class Window
{
public:
	bool Create(int windowWidth, int windowHeight, const std::wstring& titleName, const std::wstring& windowClassName);

	bool ProcessMessage();

private:
	HWND m_hWnd = {};
};
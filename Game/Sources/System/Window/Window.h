#pragma once

class Window
{
public:
	bool Create(int width, int height, const std::wstring& titleName, const std::wstring& windowClassName);

	bool ProcessMessage();

private:
	HWND m_hWnd = {};
};
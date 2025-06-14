#pragma once

class Window
{
public:
	// ウィンドウ作成
	bool Create(int windowWidth, int windowHeight, const std::wstring& titleName, const std::wstring& windowClassName);

	// ウィンドウハンドル取得
	inline HWND GetWndHandle()const { return m_hWnd; }

	bool ProcessMessage();

	void Terminate() const;

private:
	HWND m_hWnd = {};
	WNDCLASSEX m_wndClass = {};
};
#include "Application.h"

void Application::Execute()
{
	// �E�B���h�E�쐬
	if (!m_window.Create(1280, 720, L"FrameworkDX12", L"Window"))
	{
		assert(0 && "�E�B���h�E�쐬���s");
		return;
	}

	// �O���t�B�b�N�X�f�o�C�X������
	if (!GraphicsDevice::GetInstance().Init(m_window.GetWndHandle(),1280,720))
	{
		assert(0 && "�O���t�B�b�N�X�f�o�C�X���������s");
		return;
	}

	// ���C���Q�[�����[�v
	while (true)
	{
		if (!m_window.ProcessMessage())
		{
			break;
		}

		GraphicsDevice::GetInstance().ScreenFlip();
	}
}

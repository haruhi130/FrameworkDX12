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

	Mesh mesh;
	mesh.Create();

	RenderingSetting rs = {};
	rs.InputLayouts = { InputLayout::POSITION };
	rs.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };
	rs.IsDepth = false;
	rs.IsDepthMask = false;

	Shader shader;
	shader.Create(L"SimpleShader", rs, {});

	// ���C���Q�[�����[�v
	while (true)
	{
		if (!m_window.ProcessMessage())
		{
			break;
		}

		GraphicsDevice::GetInstance().Prepare();

		shader.Begin(1280, 720);
		shader.DrawMesh(mesh);

		GraphicsDevice::GetInstance().ScreenFlip();
	}
}

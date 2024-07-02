#include "Application.h"

void Application::Execute()
{
	// ウィンドウ作成
	if (!m_window.Create(1280, 720, L"FrameworkDX12", L"Window"))
	{
		assert(0 && "ウィンドウ作成失敗");
		return;
	}

	// グラフィックスデバイス初期化
	if (!GraphicsDevice::GetInstance().Init(m_window.GetWndHandle(),1280,720))
	{
		assert(0 && "グラフィックスデバイス初期化失敗");
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

	// メインゲームループ
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

#include "Application.h"

bool Application::Init(int width, int height)
{
	// メモリリーク検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	SetDirectoryAndLoadDll();

	// ウィンドウ作成
	if (!m_window.Create(width, height, L"FrameworkDX12", L"Window"))
	{
		assert(0 && "ウィンドウ作成失敗");
		return false;
	}

	// グラフィックスデバイス初期化
	if (!GraphicsDevice::GetInstance().Init(m_window.GetWndHandle(), width, height))
	{
		assert(0 && "グラフィックスデバイス初期化失敗");
		return false;
	}

	return true;
}

void Application::Execute()
{
	if (!Init())
	{
		assert(0 && "ゲーム初期化失敗");
		return;
	}

	ModelData model;
	model.Load("Assets/Models/Cube/Cube.gltf");

	Math::Matrix mWorld;

	Math::Matrix mTempWorld = Math::Matrix::CreateTranslation(1, 1, 1);

	RenderingSetting rs = {};
	rs.InputLayouts = 
	{ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::COLOR,InputLayout::NORMAL,InputLayout::TANGENT};
	rs.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Shader shader;
	shader.Create(L"SimpleShader", rs, 
		{ RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV });

	Math::Vector3 cam = {0,0,3};

	Math::Matrix mView = Math::Matrix::CreateTranslation(cam);

	Math::Matrix mProj = DirectX::XMMatrixPerspectiveFovLH
	(DirectX::XMConvertToRadians(60.0f), 1280.0f / 720.0f, 0.01f, 1000.0f);

	ConstantBufferData::Camera cbCamera;
	cbCamera.mView = mView;
	cbCamera.mProj = mProj;

	// メインゲームループ
	while (true)
	{
		if (!m_window.ProcessMessage())
		{
			break;
		}

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			break;
		}

		GraphicsDevice::GetInstance().Prepare();

		GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

		GraphicsDevice::GetInstance().GetConstantBufferAllocator()->ResetCurrentUseNumber();

		shader.Begin(1280, 720);

		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(0, cbCamera);

		if (GetAsyncKeyState('W') & 0x8000)
		{
			cam.y += 0.1f;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			cam.y -= 0.1f;
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			cam.x += 0.1f;
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			cam.x -= 0.1f;
		}
		if (GetAsyncKeyState('X') & 0x8000)
		{
			cam.z += 0.1f;
		}
		if (GetAsyncKeyState('Z') & 0x8000)
		{
			cam.z -= 0.1f;
		}

		mView = Math::Matrix::CreateTranslation(cam);
		cbCamera.mView = mView;


		mWorld *= Math::Matrix::CreateRotationY(0.01f);
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(1, mWorld);

		shader.DrawModel(model);

		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(1, mTempWorld);

		shader.DrawModel(model);

		GraphicsDevice::GetInstance().ScreenFlip();
	}
}

void Application::Terminate()
{
	// COM解放
	CoUninitialize();
	// ウィンドウ登録解除
	m_window.Terminate();
}

void Application::SetDirectoryAndLoadDll()
{
#ifdef _DEBUG
	SetDllDirectoryA("../Libraries/assimp/bin/Debug");
	LoadLibraryExA("assimp-vc143-mtd.dll", NULL, NULL);
#else
	SetDllDirectoryA("../Libraries/assimp/bin/Release");
	LoadLibraryExA("assimp-vc143-mt.dll", NULL, NULL);
#endif 
}

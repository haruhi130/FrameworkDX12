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

	// オーディオ初期化
	if (!Audio::GetInstance().Init())
	{
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

	// モデル読み込み
	std::shared_ptr<ModelData> model1 = std::make_shared<ModelData>();
	std::shared_ptr<ModelData> model2 = std::make_shared<ModelData>();
	model1->Load("Assets/Models/Cube/Cube.gltf");
	model2->Load("Assets/Models/ShortSword/Tanto.gltf");

	Math::Matrix mWorld;
	Math::Matrix mTempWorld = Math::Matrix::CreateTranslation(1, 1, 1);

	// シェーダーセット
	RenderingSetting rs = {};
	rs.InputLayouts = 
	{ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::COLOR,InputLayout::NORMAL,InputLayout::TANGENT};
	rs.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Shader shader;
	shader.Create(L"SimpleShader", rs, 
		{ RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV });

	// カメラ処理
	Math::Vector3 cam = {0,0,3};

	Math::Matrix mView = Math::Matrix::CreateTranslation(cam);

	Math::Matrix mProj = DirectX::XMMatrixPerspectiveFovLH
	(DirectX::XMConvertToRadians(60.0f), 1280.0f / 720.0f, 0.01f, 1000.0f);

	ConstantBufferData::Camera cbCamera;
	cbCamera.mView = mView;
	cbCamera.mProj = mProj;

	// アニメーション処理
	Animator animator;
	animator.SetAnimation(model1->GetAnimation(0));
	int count = 100;

	// 音再生
	WaveData waveData;
	Audio::GetInstance().PlayWaveSound(L"Assets/Sounds/TitleBGM.wav", &waveData, true);

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

		animator.ProgressTime(model1->WorkNodes(), 5.0f);

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

		if (GetAsyncKeyState('O') & 0x8000)
		{
			Audio::GetInstance().Stop();
		}
		if (GetAsyncKeyState('P') & 0x8000)
		{
			Audio::GetInstance().Pause();
		}
		if (GetAsyncKeyState('I') & 0x8000)
		{
			Audio::GetInstance().Resume();
		}
		if (GetAsyncKeyState('U') & 0x8000)
		{
			Audio::GetInstance().ExitLoop();
		}


		mView = Math::Matrix::CreateTranslation(cam);
		cbCamera.mView = mView;

		if (GetAsyncKeyState('R') & 0x8000)
		{
			if (count < 0)
			{
				count = 100;
				model1.swap(model2);
			}
		}

		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(1, model1->GetNodes()[0].mLocal * mWorld);

		shader.DrawModel(*model1);

		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(1, model2->GetNodes()[0].mLocal * mTempWorld);

		shader.DrawModel(*model2);

		GraphicsDevice::GetInstance().ScreenFlip();

		--count;
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

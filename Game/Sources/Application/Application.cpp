#include "Application.h"

bool Application::Init(int width, int height)
{
	// メモリリーク検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

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

	// DLL設定(Assimp用)
	SetDirectoryAndLoadDll();

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
	std::shared_ptr<ModelWork> model1 = std::make_shared<ModelWork>();
	std::shared_ptr<ModelWork> model2 = std::make_shared<ModelWork>();
	model1->SetModelData("Assets/Models/Cube/Cube.gltf");
	model2->SetModelData("Assets/Models/Character/3DChara.gltf");

	// 当たり判定
	std::shared_ptr<Collider> col = std::make_shared<Collider>();
	col->RegisterCollisionShape("Tanto",model2, Collider::Type::Bump);

	Math::Matrix mWorld;

	// モデル2用
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(1, 1, 1);
	
	// シェーダーセット
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::COLOR,InputLayout::NORMAL,InputLayout::TANGENT };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Shader shader;
	shader.Create(L"SimpleShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV });

	// カメラ処理
	Math::Vector3 cam = { 0,0,10 };

	Camera camera;
	camera.SetCameraMatrix(Math::Matrix::CreateTranslation(cam));

	// アニメーション処理
	Animator animator;
	animator.SetAnimation(model1->GetAnimation("CubeAction"));
	//Animator anime;
	//anime.SetAnimation(model2->GetAnimation("Walk"));

	float animationSpeed = 5.0f;

	// 音再生
	Audio::GetInstance().PlayWaveSound(L"Assets/Sounds/TitleBGM.wav", true);

	// 可変フレームレート
	ServiceLocator::Add(std::make_shared<Time>());
	auto time = ServiceLocator::Get<Time>();
	if (time != nullptr) { time->Start(); }

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

		animator.ProgressTime(model1->WorkNodes(), animationSpeed);
		//anime.ProgressTime(model2->WorkNodes(), animationSpeed);

		GraphicsDevice::GetInstance().Prepare();

		// 画像用にヒープを指定
		GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

		// コンスタントバッファ初期化
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()->ResetCurrentUseNumber();

		// Shader処理
		shader.Begin(1280, 720);

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

		// カメラ設定
		camera.SetCameraMatrix(Math::Matrix::CreateTranslation(cam));
		camera.Set();


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

		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(1, model1->GetNodes()[0].mLocal * mWorld);

		shader.DrawModel(*model1);

		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(1, model2->GetNodes()[0].mLocal * mTrans);

		shader.DrawModel(*model2);

		GraphicsDevice::GetInstance().ScreenFlip();

		time->Update();
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

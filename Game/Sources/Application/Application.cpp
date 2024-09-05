#include "Application.h"

#include "Scene/SceneManager.h"

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

	// シーン初期化
	SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Game);

	return true;
}

void Application::Execute()
{
	// ゲーム初期化
	if (!Init())
	{
		assert(0 && "ゲーム初期化失敗");
		return;
	}

	//===============================================
	// 仮実装

	// モデル読み込み
	std::shared_ptr<ModelWork> model1 = std::make_shared<ModelWork>();
	std::shared_ptr<ModelWork> model2 = std::make_shared<ModelWork>();
	model1->SetModelData("Assets/Models/Cube/Cube.gltf");
	model2->SetModelData("Assets/Models/Character/3DChara.gltf");

	// 当たり判定
	std::shared_ptr<Collider> col = std::make_shared<Collider>();
	col->RegisterCollisionShape("Tanto",model2, Collider::Type::Bump);

	// モデル1用
	Math::Vector3 playerVec = {};
	Math::Matrix mWorld;

	// モデル2用
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(1, 1, 1);

	Math::Matrix mTWorld = mRot * mTrans;

	// シェーダーに渡す情報設定
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::COLOR,InputLayout::NORMAL,InputLayout::TANGENT };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	// シェーダー作成(どのシェーダーを使用するか)
	Shader shader;
	shader.Create(L"SimpleShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV });

	// カメラ行列
	Math::Vector3 cam = { 0,0,10 };

	// カメラ本体
	Camera camera;
	
	// アニメーション
	Animator animator;
	animator.SetAnimation(model1->GetAnimation("CubeAction"));
	Animator anime;
	anime.SetAnimation(model2->GetAnimation("Walk"));

	// アニメーション速度
	float animationSpeed = 1.0f;

	// 音再生
	Audio::GetInstance().PlayWaveSound(L"Assets/Sounds/TitleBGM.wav", true);

	// 可変フレームレート
	ServiceLocator::Add(std::make_shared<Time>());
	auto time = ServiceLocator::Get<Time>();
	if (time != nullptr) { time->Start(); }

	//===============================================

	// メインゲームループ
	while (true)
	{
		// ウィンドウメッセージ処理
		if (!m_window.ProcessMessage())
		{
			break;
		}

		// 終了
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			break;
		}

		//=============================================
		// 
		// アプリケーション更新
		// 
		//=============================================
		PreUpdate();

		Update();
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

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			playerVec.x += 0.1f;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			playerVec.x -= 0.1f;
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			playerVec.y += 0.1f;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			playerVec.y -= 0.1f;
		}

		if (GetAsyncKeyState('M') & 0x8000)
		{
			playerVec.z += 0.1f;
		}
		if (GetAsyncKeyState('N') & 0x8000)
		{
			playerVec.z -= 0.1f;
		}

		mWorld.Translation(playerVec);

		PostUpdate();
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

		animator.ProgressTime(model1->WorkNodes(), animationSpeed);
		anime.ProgressTime(model2->WorkNodes(), animationSpeed);

		//=============================================
		// 
		// アプリケーション描画
		// 
		//=============================================
		GraphicsDevice::GetInstance().Prepare();

		// 画像用にヒープを指定
		GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

		// コンスタントバッファ初期化
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()->ResetCurrentUseNumber();

		// Shader処理
		shader.Begin(1280, 720);

		PreDraw();
		// カメラ設定
		camera.SetCameraMatrix(Math::Matrix::CreateTranslation(cam));
		camera.Set();

		Draw();

		shader.DrawModel(*model1,mWorld);

		shader.DrawModel(*model2, mTWorld);

		//=============================================
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

void Application::PreUpdate()
{
	SceneManager::GetInstance().PreUpdate();
}

void Application::Update()
{
	SceneManager::GetInstance().Update();
}

void Application::PostUpdate()
{
	SceneManager::GetInstance().PostUpdate();
}

void Application::PreDraw()
{
	SceneManager::GetInstance().PreDraw();
}

void Application::Draw()
{
	SceneManager::GetInstance().Draw();
}

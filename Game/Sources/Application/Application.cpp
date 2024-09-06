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
	model2->SetModelData("Assets/Models/Mouse/Mouse.gltf");

	// 当たり判定
	std::shared_ptr<Collider> col = std::make_shared<Collider>();
	col->RegisterCollisionShape("Tanto",model2, Collider::Type::Bump);

	// モデル1用
	Math::Vector3 playerVec = {};
	Math::Matrix mWorld;

	// モデル2用
	Math::Matrix mRot = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));
	Math::Matrix mScale = Math::Matrix::CreateScale(5000.0f);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(1, 1, 1);
	Math::Matrix mTWorld = mRot * mScale * mTrans;

	// シェーダーに渡す情報設定
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::COLOR,InputLayout::NORMAL,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	// シェーダー作成(どのシェーダーを使用するか)
	Shader shader;
	shader.Create(L"SimpleShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV });

	// アニメーション
	Animator animator;
	animator.SetAnimation(model1->GetAnimation("CubeAction"));
	Animator anime;
	anime.SetAnimation(model2->GetAnimation("Idle"));

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
		
		// 通常の更新前に行う処理
		// シーン切り替えやイテレータ処理等
		PreUpdate();

		// 通常の更新
		// キャラの移動や判定等
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

		// 通常の更新終了後に行う処理
		// カメラの移動やアニメーション等
		PostUpdate();
		
		// アニメーション実行
		animator.ProgressTime(model1->WorkNodes());
		// 行列計算で実際に移動させる
		model1->CalcNodeMatrices();

		anime.ProgressTime(model2->WorkNodes());
		model2->CalcNodeMatrices();

		//=============================================
		// 
		// アプリケーション描画
		// 
		//=============================================
		
		// 描画準備開始
		GraphicsDevice::GetInstance().Prepare();

		// 画像用にヒープを指定
		GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

		// コンスタントバッファ初期化
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()->ResetCurrentUseNumber();

		// Shader処理
		shader.Begin();

		// 事前描画
		// カメラ等
		PreDraw();

		// 通常の描画
		// モデル等
		Draw();
		shader.DrawModel(*model1,mWorld);
		shader.DrawModel(*model2, mTWorld);

		//=============================================
		
		// 描画終了
		// ダブルバッファリングを行う
		GraphicsDevice::GetInstance().ScreenFlip();

		// 時間管理
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
	//SceneManager::GetInstance().PreUpdate();
}

void Application::Update()
{
	//SceneManager::GetInstance().Update();
}

void Application::PostUpdate()
{
	//SceneManager::GetInstance().PostUpdate();
}

void Application::PreDraw()
{
	//SceneManager::GetInstance().PreDraw();
}

void Application::Draw()
{
	//SceneManager::GetInstance().Draw();
}

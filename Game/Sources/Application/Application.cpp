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

	// 当たり判定
	//std::shared_ptr<Collider> col = std::make_shared<Collider>();
	//col->RegisterCollisionShape("Tanto",model2, Collider::Type::Bump);

	// シェーダー作成(どのシェーダーを使用するか)
	m_spShader = m_spShader->CreateSimpleShader();

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
		PreUpdate();

		// 通常の更新
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

		// 通常の更新終了後に行う処理
		PostUpdate();

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

		m_spShader->Begin();
		
		// 事前描画
		PreDraw();

		// 通常の描画
		Draw();

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

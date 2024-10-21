#include "Application.h"

#include "Scene/SceneManager.h"

bool Application::Init(int width, int height)
{
	// メモリリーク検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//===============================================
	// COM初期化
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//===============================================
	// ウィンドウ作成
	if (!m_window.Create(width, height, L"FrameworkDX12", L"Window"))
	{
		assert(0 && "ウィンドウ作成失敗");
		return false;
	}

	//===============================================
	// グラフィックスデバイス初期化
	if (!GraphicsDevice::GetInstance().Init(m_window.GetWndHandle(), width, height))
	{
		assert(0 && "グラフィックスデバイス初期化失敗");
		return false;
	}

	//===============================================
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	
	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_window.GetWndHandle());
	ImGui_ImplDX12_Init(
		GraphicsDevice::GetInstance().GetDevice(),
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		GraphicsDevice::GetInstance().GetImGuiHeap()->GetHeap().Get(),
		GraphicsDevice::GetInstance().GetImGuiHeap()->GetHeap()->GetCPUDescriptorHandleForHeapStart(),
		GraphicsDevice::GetInstance().GetImGuiHeap()->GetHeap()->GetGPUDescriptorHandleForHeapStart()
		);

#include "../imgui/ja_glyph_ranges.h"
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	// ImGui日本語対応
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);

	//===============================================
	// シェーダー初期化
	ShaderManager::GetInstance().Init();

	//===============================================
	// オーディオ初期化
	if (!Audio::GetInstance().Init())
	{
		return false;
	}

	//===============================================
	// シーン初期化
	SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Title);

	return true;
}

void Application::Execute()
{
	// 可変フレームレート対応
	ServiceLocator::Add(std::make_shared<Time_VRR>());

	// ゲーム初期化
	if (!Init())
	{
		assert(0 && "ゲーム初期化失敗");
		return;
	}

	// 時間管理
	std::shared_ptr<Time> spTime = ServiceLocator::Get<Time>();
	if (spTime != nullptr) { spTime->Start(); }

	// 音再生
	Audio::GetInstance().PlayWaveSound(L"Assets/Sounds/Under_line.wav", true);

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

		// 事前更新
		PreUpdate();

		// 通常更新
		Update();
		if (GetAsyncKeyState('V') & 0x8000)
		{
			Audio::GetInstance().Stop();
		}
		if (GetAsyncKeyState('B') & 0x8000)
		{
			Audio::GetInstance().Pause();
		}
		if (GetAsyncKeyState('N') & 0x8000)
		{
			Audio::GetInstance().Resume();
		}
		if (GetAsyncKeyState('M') & 0x8000)
		{
			Audio::GetInstance().ExitLoop();
		}

		// 後更新
		PostUpdate();

		//=============================================
		// 
		// アプリケーション描画
		// 
		//=============================================

		PreDraw();

		Draw();

		PostDraw();

		DrawSprite();

		ShaderManager::GetInstance().m_postProcessShader.Begin();
		ShaderManager::GetInstance().m_postProcessShader.Draw();

		//=============================================
		// ImGui処理
		//ImGuiUpdate();

		// 時間管理
		ServiceLocator::Update();

		//=============================================
		// 描画終了/バッファフリップ
		GraphicsDevice::GetInstance().ScreenFlip();
	}
}

void Application::Terminate()
{
	// ImGui Release
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

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
	// レンダーターゲット変更
	ShaderManager::GetInstance().m_postProcessShader.PreDraw();

	// 画像用にヒープを指定
	GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

	// コンスタントバッファ初期化
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()->ResetCurrentUseNumber();

	SceneManager::GetInstance().PreDraw();
}

void Application::Draw()
{
	SceneManager::GetInstance().Draw();
}

void Application::PostDraw()
{
	ShaderManager::GetInstance().m_postProcessShader.PostProcess();

	// 描画準備開始
	GraphicsDevice::GetInstance().Prepare();
}

void Application::DrawSprite()
{
	SceneManager::GetInstance().DrawSprite();
}

void Application::ImGuiUpdate()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	SceneManager::GetInstance().ImGuiUpdate();

	ImGui::Render();

	GraphicsDevice::GetInstance().GetCmdList()->SetDescriptorHeaps(
		1, GraphicsDevice::GetInstance().GetImGuiHeap()->GetHeap().GetAddressOf());

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(),
		GraphicsDevice::GetInstance().GetCmdList());
}

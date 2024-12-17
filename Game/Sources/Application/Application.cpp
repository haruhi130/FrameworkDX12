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
	if (!m_window.Create(width, height, L"ネズミ怪盗カフェ・ラッテ", L"Window"))
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
	if (!ShaderManager::GetInstance().Init())
	{
		return false;
	}

	//===============================================
	// オーディオ初期化
	if (!AudioManager::GetInstance().Init())
	{
		return false;
	};

	//===============================================
	// Effekseer初期化
	if (!EffekseerManager::GetInstance().Init(width, height))
	{
		return false;
	}

	//===============================================
	// 入力管理初期化
	{
		// 入力デバイス登録
		InputCollector* keyboadCollector = new InputCollector();
		InputManager::GetInstance().AddDevice("Windows", keyboadCollector);

		// ボタン登録
		keyboadCollector->AddButton("Up", new InputButtonForWindows({ 'W',VK_UP }));
		keyboadCollector->AddButton("Left", new InputButtonForWindows({ 'A',VK_LEFT }));
		keyboadCollector->AddButton("Down", new InputButtonForWindows({ 'S',VK_DOWN }));
		keyboadCollector->AddButton("Right", new InputButtonForWindows({ 'D' ,VK_RIGHT }));

		keyboadCollector->AddButton("LClick", new InputButtonForWindows({ VK_LBUTTON }));
		keyboadCollector->AddButton("RClick", new InputButtonForWindows({ VK_RBUTTON }));

		keyboadCollector->AddButton("Return", new InputButtonForWindows({ VK_RETURN }));

		keyboadCollector->AddButton("Escape", new InputButtonForWindows({ VK_ESCAPE }));
		keyboadCollector->AddButton("Alt", new InputButtonForWindows({ VK_MENU }));
	}

	//===============================================
	// シーン初期化
	SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Title);

	return true;
}

void Application::Execute()
{
	// 可変フレームレート対応
	ServiceLocator::Add(std::make_shared<Time>());

	// ゲーム初期化
	if (!Init())
	{
		assert(0 && "ゲーム初期化失敗");
		return;
	}

	// 時間管理
	auto spTime = ServiceLocator::Get<Time>();
	if (spTime) { spTime->Start(); }

	// 音再生
	auto bgm = AudioManager::GetInstance().Play("Assets/Sounds/Under_line.wav", true);
	bgm->SetVolume(SceneManager::GetInstance().GetBGMVolume());

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
		if (InputManager::GetInstance().IsRelease("Escape"))
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

		//=============================================
		// ImGui処理
		//ImGuiUpdate();

		// サービス管理更新
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
	// 入力状況の更新
	InputManager::GetInstance().Update();

	SceneManager::GetInstance().PreUpdate();
}

void Application::Update()
{
	// サウンド更新
	AudioManager::GetInstance().Update();

	SceneManager::GetInstance().Update();
}

void Application::PostUpdate()
{
	// 3Dサウンド用リスナー更新
	AudioManager::GetInstance().SetListenerMatrix(
		ShaderManager::GetInstance().GetCBCamera().mView.Invert());

	SceneManager::GetInstance().PostUpdate();
}

void Application::PreDraw()
{
	// コンスタントバッファ番号初期化
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()->ResetCurrentUseNumber();

	SceneManager::GetInstance().PreDraw();

	ShaderManager::GetInstance().m_postProcessShader.PreDraw();
}

void Application::Draw()
{
	SceneManager::GetInstance().Draw();
}

void Application::PostDraw()
{
	ShaderManager::GetInstance().m_postProcessShader.PostProcess();

	// 描画前準備
	GraphicsDevice::GetInstance().Prepare();

	ShaderManager::GetInstance().m_postProcessShader.Begin();

	ShaderManager::GetInstance().m_postProcessShader.Draw();
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

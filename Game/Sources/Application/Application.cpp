#include "Application.h"

#include "Scene/SceneManager.h"

bool Application::Init(int width, int height)
{
	// ���������[�N���m
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//===============================================
	// COM������
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//===============================================
	// �E�B���h�E�쐬
	if (!m_window.Create(width, height, L"�l�Y�~�����J�t�F�E���b�e", L"Window"))
	{
		assert(0 && "�E�B���h�E�쐬���s");
		return false;
	}

	//===============================================
	// �O���t�B�b�N�X�f�o�C�X������
	if (!GraphicsDevice::GetInstance().Init(m_window.GetWndHandle(), width, height))
	{
		assert(0 && "�O���t�B�b�N�X�f�o�C�X���������s");
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
	// ImGui���{��Ή�
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);

	//===============================================
	// �V�F�[�_�[������
	if (!ShaderManager::GetInstance().Init())
	{
		return false;
	}

	//===============================================
	// �I�[�f�B�I������
	if (!AudioManager::GetInstance().Init())
	{
		return false;
	};

	//===============================================
	// Effekseer������
	if (!EffekseerManager::GetInstance().Init(width, height))
	{
		return false;
	}

	//===============================================
	// ���͊Ǘ�������
	{
		// ���̓f�o�C�X�o�^
		InputCollector* keyboadCollector = new InputCollector();
		InputManager::GetInstance().AddDevice("Windows", keyboadCollector);

		// �{�^���o�^
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
	// �V�[��������
	SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Title);

	return true;
}

void Application::Execute()
{
	// �σt���[�����[�g�Ή�
	ServiceLocator::Add(std::make_shared<Time>());

	// �Q�[��������
	if (!Init())
	{
		assert(0 && "�Q�[�����������s");
		return;
	}

	// ���ԊǗ�
	auto spTime = ServiceLocator::Get<Time>();
	if (spTime) { spTime->Start(); }

	// ���Đ�
	auto bgm = AudioManager::GetInstance().Play("Assets/Sounds/Under_line.wav", true);
	bgm->SetVolume(SceneManager::GetInstance().GetBGMVolume());

	//===============================================
	// ���C���Q�[�����[�v
	while (true)
	{
		// �E�B���h�E���b�Z�[�W����
		if (!m_window.ProcessMessage())
		{
			break;
		}

		// �I��
		if (InputManager::GetInstance().IsRelease("Escape"))
		{
			break;
		}

		//=============================================
		// 
		// �A�v���P�[�V�����X�V
		// 
		//=============================================

		PreUpdate();

		Update();

		PostUpdate();

		//=============================================
		// 
		// �A�v���P�[�V�����`��
		// 
		//=============================================

		PreDraw();

		Draw();

		PostDraw();

		DrawSprite();

		//=============================================
		// ImGui����
		//ImGuiUpdate();

		// �T�[�r�X�Ǘ��X�V
		ServiceLocator::Update();

		//=============================================
		// �`��I��/�o�b�t�@�t���b�v
		GraphicsDevice::GetInstance().ScreenFlip();
	}
}

void Application::Terminate()
{
	// ImGui Release
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// COM���
	CoUninitialize();

	// �E�B���h�E�o�^����
	m_window.Terminate();
}

void Application::PreUpdate()
{
	// ���͏󋵂̍X�V
	InputManager::GetInstance().Update();

	SceneManager::GetInstance().PreUpdate();
}

void Application::Update()
{
	// �T�E���h�X�V
	AudioManager::GetInstance().Update();

	SceneManager::GetInstance().Update();
}

void Application::PostUpdate()
{
	// 3D�T�E���h�p���X�i�[�X�V
	AudioManager::GetInstance().SetListenerMatrix(
		ShaderManager::GetInstance().GetCBCamera().mView.Invert());

	SceneManager::GetInstance().PostUpdate();
}

void Application::PreDraw()
{
	// �R���X�^���g�o�b�t�@�ԍ�������
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

	// �`��O����
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

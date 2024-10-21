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
	if (!m_window.Create(width, height, L"FrameworkDX12", L"Window"))
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
	ShaderManager::GetInstance().Init();

	//===============================================
	// �I�[�f�B�I������
	if (!Audio::GetInstance().Init())
	{
		return false;
	}

	//===============================================
	// �V�[��������
	SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Title);

	return true;
}

void Application::Execute()
{
	// �σt���[�����[�g�Ή�
	ServiceLocator::Add(std::make_shared<Time_VRR>());

	// �Q�[��������
	if (!Init())
	{
		assert(0 && "�Q�[�����������s");
		return;
	}

	// ���ԊǗ�
	std::shared_ptr<Time> spTime = ServiceLocator::Get<Time>();
	if (spTime != nullptr) { spTime->Start(); }

	// ���Đ�
	Audio::GetInstance().PlayWaveSound(L"Assets/Sounds/Under_line.wav", true);

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
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			break;
		}

		//=============================================
		// 
		// �A�v���P�[�V�����X�V
		// 
		//=============================================

		// ���O�X�V
		PreUpdate();

		// �ʏ�X�V
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

		// ��X�V
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

		ShaderManager::GetInstance().m_postProcessShader.Begin();
		ShaderManager::GetInstance().m_postProcessShader.Draw();

		//=============================================
		// ImGui����
		//ImGuiUpdate();

		// ���ԊǗ�
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
	// �����_�[�^�[�Q�b�g�ύX
	ShaderManager::GetInstance().m_postProcessShader.PreDraw();

	// �摜�p�Ƀq�[�v���w��
	GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

	// �R���X�^���g�o�b�t�@������
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

	// �`�揀���J�n
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

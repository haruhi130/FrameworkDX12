#include "Application.h"

#include "Scene/SceneManager.h"

bool Application::Init(int width, int height)
{
	// ���������[�N���m
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM������
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// �E�B���h�E�쐬
	if (!m_window.Create(width, height, L"FrameworkDX12", L"Window"))
	{
		assert(0 && "�E�B���h�E�쐬���s");
		return false;
	}

	// �O���t�B�b�N�X�f�o�C�X������
	if (!GraphicsDevice::GetInstance().Init(m_window.GetWndHandle(), width, height))
	{
		assert(0 && "�O���t�B�b�N�X�f�o�C�X���������s");
		return false;
	}

	// �I�[�f�B�I������
	if (!Audio::GetInstance().Init())
	{
		return false;
	}

	// �V�[��������
	SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Game);

	return true;
}

void Application::Execute()
{
	// �Q�[��������
	if (!Init())
	{
		assert(0 && "�Q�[�����������s");
		return;
	}

	//===============================================
	// ������

	// �����蔻��
	//std::shared_ptr<Collider> col = std::make_shared<Collider>();
	//col->RegisterCollisionShape("Tanto",model2, Collider::Type::Bump);

	// �V�F�[�_�[�쐬(�ǂ̃V�F�[�_�[���g�p���邩)
	m_spShader = m_spShader->CreateSimpleShader();

	// ���Đ�
	Audio::GetInstance().PlayWaveSound(L"Assets/Sounds/TitleBGM.wav", true);

	// �σt���[�����[�g
	ServiceLocator::Add(std::make_shared<Time>());
	auto time = ServiceLocator::Get<Time>();
	if (time != nullptr) { time->Start(); }

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

		// �ʏ�̍X�V�O�ɍs������
		PreUpdate();

		// �ʏ�̍X�V
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

		// �ʏ�̍X�V�I����ɍs������
		PostUpdate();

		//=============================================
		// 
		// �A�v���P�[�V�����`��
		// 
		//=============================================

		// �`�揀���J�n
		GraphicsDevice::GetInstance().Prepare();

		// �摜�p�Ƀq�[�v���w��
		GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

		// �R���X�^���g�o�b�t�@������
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()->ResetCurrentUseNumber();

		m_spShader->Begin();
		
		// ���O�`��
		PreDraw();

		// �ʏ�̕`��
		Draw();

		//=============================================

		// �`��I��
		// �_�u���o�b�t�@�����O���s��
		GraphicsDevice::GetInstance().ScreenFlip();

		// ���ԊǗ�
		time->Update();
	}
}

void Application::Terminate()
{
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
	SceneManager::GetInstance().PreDraw();
}

void Application::Draw()
{
	SceneManager::GetInstance().Draw();
}

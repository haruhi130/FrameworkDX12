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

	// ���f���ǂݍ���
	std::shared_ptr<ModelWork> model1 = std::make_shared<ModelWork>();
	std::shared_ptr<ModelWork> model2 = std::make_shared<ModelWork>();
	model1->SetModelData("Assets/Models/Cube/Cube.gltf");
	model2->SetModelData("Assets/Models/Character/3DChara.gltf");

	// �����蔻��
	std::shared_ptr<Collider> col = std::make_shared<Collider>();
	col->RegisterCollisionShape("Tanto",model2, Collider::Type::Bump);

	// ���f��1�p
	Math::Vector3 playerVec = {};
	Math::Matrix mWorld;

	// ���f��2�p
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(1, 1, 1);

	Math::Matrix mTWorld = mRot * mTrans;

	// �V�F�[�_�[�ɓn�����ݒ�
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::COLOR,InputLayout::NORMAL,InputLayout::TANGENT };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	// �V�F�[�_�[�쐬(�ǂ̃V�F�[�_�[���g�p���邩)
	Shader shader;
	shader.Create(L"SimpleShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV });

	// �J�����s��
	Math::Vector3 cam = { 0,0,10 };

	// �J�����{��
	Camera camera;
	
	// �A�j���[�V����
	Animator animator;
	animator.SetAnimation(model1->GetAnimation("CubeAction"));
	Animator anime;
	anime.SetAnimation(model2->GetAnimation("Walk"));

	// �A�j���[�V�������x
	float animationSpeed = 1.0f;

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
		// �A�v���P�[�V�����`��
		// 
		//=============================================
		GraphicsDevice::GetInstance().Prepare();

		// �摜�p�Ƀq�[�v���w��
		GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

		// �R���X�^���g�o�b�t�@������
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()->ResetCurrentUseNumber();

		// Shader����
		shader.Begin(1280, 720);

		PreDraw();
		// �J�����ݒ�
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

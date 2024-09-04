#include "Application.h"

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

	// DLL�ݒ�(Assimp�p)
	SetDirectoryAndLoadDll();

	return true;
}

void Application::Execute()
{
	if (!Init())
	{
		assert(0 && "�Q�[�����������s");
		return;
	}

	// ���f���ǂݍ���
	std::shared_ptr<ModelWork> model1 = std::make_shared<ModelWork>();
	std::shared_ptr<ModelWork> model2 = std::make_shared<ModelWork>();
	model1->SetModelData("Assets/Models/Cube/Cube.gltf");
	model2->SetModelData("Assets/Models/Character/3DChara.gltf");

	// �����蔻��
	std::shared_ptr<Collider> col = std::make_shared<Collider>();
	col->RegisterCollisionShape("Tanto",model2, Collider::Type::Bump);

	Math::Matrix mWorld;

	// ���f��2�p
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(1, 1, 1);
	
	// �V�F�[�_�[�Z�b�g
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::COLOR,InputLayout::NORMAL,InputLayout::TANGENT };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Shader shader;
	shader.Create(L"SimpleShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV });

	// �J��������
	Math::Vector3 cam = { 0,0,10 };

	Camera camera;
	camera.SetCameraMatrix(Math::Matrix::CreateTranslation(cam));

	// �A�j���[�V��������
	Animator animator;
	animator.SetAnimation(model1->GetAnimation("CubeAction"));
	//Animator anime;
	//anime.SetAnimation(model2->GetAnimation("Walk"));

	float animationSpeed = 5.0f;

	// ���Đ�
	Audio::GetInstance().PlayWaveSound(L"Assets/Sounds/TitleBGM.wav", true);

	// �σt���[�����[�g
	ServiceLocator::Add(std::make_shared<Time>());
	auto time = ServiceLocator::Get<Time>();
	if (time != nullptr) { time->Start(); }

	// ���C���Q�[�����[�v
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

		// �摜�p�Ƀq�[�v���w��
		GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

		// �R���X�^���g�o�b�t�@������
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()->ResetCurrentUseNumber();

		// Shader����
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

		// �J�����ݒ�
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
	// COM���
	CoUninitialize();

	// �E�B���h�E�o�^����
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

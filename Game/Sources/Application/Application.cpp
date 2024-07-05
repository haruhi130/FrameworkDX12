#include "Application.h"

bool Application::Init(int width, int height)
{
	// ���������[�N���m
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM������
	HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

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

	return true;
}

void Application::Execute()
{
	if (!Init())
	{
		assert(0 && "�Q�[�����������s");
		return;
	}

	Mesh mesh;
	mesh.Create();

	RenderingSetting rs = {};
	rs.InputLayouts = { InputLayout::POSITION,InputLayout::TEXCOORD };
	rs.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };
	rs.IsDepth = false;
	rs.IsDepthMask = false;

	Shader shader;
	shader.Create(L"SimpleShader", rs, {RangeType::CBV,RangeType::SRV});

	Texture sampleTex;
	sampleTex.Load("Assets/Textures/back.png");

	Math::Matrix mView = Math::Matrix::CreateTranslation(0, 0, 3);

	Math::Matrix mProj = DirectX::XMMatrixPerspectiveFovLH
	(DirectX::XMConvertToRadians(60.0f), 1280.0f / 720.0f, 0.01f, 1000.0f);

	ConstantBufferData::Camera cbCamera;
	cbCamera.mView = mView;
	cbCamera.mProj = mProj;

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

		GraphicsDevice::GetInstance().Prepare();

		GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

		GraphicsDevice::GetInstance().GetConstantBufferAllocator()->ResetCurrentUseNumber();

		shader.Begin(1280, 720);

		sampleTex.Set(shader.GetCBVCount() + sampleTex.GetSRVNumber());

		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(0, cbCamera);

		shader.DrawMesh(mesh);

		GraphicsDevice::GetInstance().ScreenFlip();
	}
}

void Application::Terminate()
{
	// COM���
	CoUninitialize();
	// �E�B���h�E�o�^����
	m_window.Terminate();
}

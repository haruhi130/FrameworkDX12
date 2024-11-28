#include "EffekseerManager.h"

bool EffekseerManager::Init()
{
	DXGI_FORMAT bbFormats[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
	m_pEfkRenderer = EffekseerRendererDX12::Create(
		GraphicsDevice::GetInstance().GetDevice(),
		GraphicsDevice::GetInstance().GetCmdQueue(),
		2, bbFormats, 1, DXGI_FORMAT_UNKNOWN, false, 10000);

	if (!m_pEfkRenderer) { return false; }

	m_pEfkManager = Effekseer::Manager::Create(10000);

	if (!m_pEfkManager) { return false; }

	m_pEfkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// 描画用インスタンスから描画機能を設定
	m_pEfkManager->SetSpriteRenderer(m_pEfkRenderer->CreateSpriteRenderer());
	m_pEfkManager->SetRibbonRenderer(m_pEfkRenderer->CreateRibbonRenderer());
	m_pEfkManager->SetRingRenderer(m_pEfkRenderer->CreateRingRenderer());
	m_pEfkManager->SetTrackRenderer(m_pEfkRenderer->CreateTrackRenderer());
	m_pEfkManager->SetModelRenderer(m_pEfkRenderer->CreateModelRenderer());

	m_pEfkManager->SetTextureLoader(m_pEfkRenderer->CreateTextureLoader());
	m_pEfkManager->SetModelLoader(m_pEfkRenderer->CreateModelLoader());
	m_pEfkManager->SetMaterialLoader(m_pEfkRenderer->CreateMaterialLoader());
	m_pEfkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	m_pEfkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(m_pEfkRenderer->GetGraphicsDevice());
	m_pEfkCmdList = EffekseerRenderer::CreateCommandList(m_pEfkRenderer->GetGraphicsDevice(), m_pEfkMemoryPool);

	m_pEfkRenderer->SetCommandList(m_pEfkCmdList);

	m_pEfkRenderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		90.0f / 180.0f * 3.14f, (float)1280 / (float)720, 1, 500.0f));

	return true;
}

bool EffekseerManager::Load()
{
	m_pEffect = Effekseer::Effect::Create(m_pEfkManager,
		(const EFK_CHAR*)sjis_to_wide("Assets/Effects/10/SimpleLaser.efk").c_str(), 1.0f);

	if (!m_pEffect) { return false; }

	//m_efkHandle = m_pEfkManager->Play(m_pEffect, 0, 0.1f, 10.0f);

	return true;
}

void EffekseerManager::Play()
{
	m_efkHandle = m_pEfkManager->Play(m_pEffect, 0, 0.1f, 10.0f);
}

void EffekseerManager::Update()
{
	if (!m_pEfkManager) { return; }

	m_pEfkManager->Update();

	std::shared_ptr<Camera> spCamera = m_wpCamera.lock();
	if (!spCamera) { return; }

	Math::Matrix mView = spCamera->GetCameraMatrix().Invert();
	Math::Matrix mProj = spCamera->GetProjMatrix();

	Effekseer::Matrix44 mEfkView, mEfkProj;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mEfkView.Values[i][j] = mView.m[i][j];
			mEfkProj.Values[i][j] = mProj.m[i][j];
		}
	}

	m_pEfkRenderer->SetCameraMatrix(mEfkView);
	m_pEfkRenderer->SetProjectionMatrix(mEfkProj);
}

void EffekseerManager::Draw()
{
	if (!m_pEfkManager || !m_pEfkRenderer || 
		!m_pEfkMemoryPool || !m_pEfkCmdList) { return; }

	m_pEfkMemoryPool->NewFrame();

	EffekseerRendererDX12::BeginCommandList(m_pEfkCmdList, GraphicsDevice::GetInstance().GetCmdList());

	m_pEfkRenderer->BeginRendering();
	m_pEfkManager->Draw();
	m_pEfkRenderer->EndRendering();
	EffekseerRendererDX12::EndCommandList(m_pEfkCmdList);
}

void EffekseerManager::End()
{
	//m_pEfkRenderer->EndRendering();
	EffekseerRendererDX12::EndCommandList(m_pEfkCmdList);
}

void EffekseerManager::Release()
{
	m_pEfkManager.Reset();
	m_pEfkRenderer.Reset();

	m_pEfkMemoryPool.Reset();
	m_pEfkCmdList.Reset();
}

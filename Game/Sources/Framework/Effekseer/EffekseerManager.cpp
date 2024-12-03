#include "EffekseerManager.h"

bool EffekseerManager::Init(int w, int h)
{
	// �����_���[�쐬
	DXGI_FORMAT bbFormats[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
	m_pEfkRenderer = EffekseerRendererDX12::Create(
		GraphicsDevice::GetInstance().GetDevice(),
		GraphicsDevice::GetInstance().GetCmdQueue(),
		2, bbFormats, 1, DXGI_FORMAT_D32_FLOAT, false, 10000);
	if (!m_pEfkRenderer) { return false; }

	// �}�l�[�W���[�쐬
	m_pEfkManager = Effekseer::Manager::Create(10000);
	if (!m_pEfkManager) { return false; }

	// ������W�n�ɐݒ�
	m_pEfkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	m_pEfkManager->SetSpriteRenderer(m_pEfkRenderer->CreateSpriteRenderer());
	m_pEfkManager->SetRibbonRenderer(m_pEfkRenderer->CreateRibbonRenderer());
	m_pEfkManager->SetRingRenderer(m_pEfkRenderer->CreateRingRenderer());
	m_pEfkManager->SetTrackRenderer(m_pEfkRenderer->CreateTrackRenderer());
	m_pEfkManager->SetModelRenderer(m_pEfkRenderer->CreateModelRenderer());

	// �`��p�C���X�^���X����Ǎ��@�\��ݒ�
	m_pEfkManager->SetTextureLoader(m_pEfkRenderer->CreateTextureLoader());
	m_pEfkManager->SetModelLoader(m_pEfkRenderer->CreateModelLoader());
	m_pEfkManager->SetMaterialLoader(m_pEfkRenderer->CreateMaterialLoader());
	m_pEfkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// �������v�[���쐬
	m_pEfkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(m_pEfkRenderer->GetGraphicsDevice());
	if (!m_pEfkMemoryPool) { return false; }

	// �R�}���h���X�g�쐬
	m_pEfkCmdList = EffekseerRenderer::CreateCommandList(m_pEfkRenderer->GetGraphicsDevice(), m_pEfkMemoryPool);
	if (!m_pEfkCmdList) { return false; }

	// �R�}���h���X�g�ݒ�
	m_pEfkRenderer->SetCommandList(m_pEfkCmdList);

	// ���e�s��ݒ�
	m_pEfkRenderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		90.0f / 180.0f * 3.14f, (float)w / (float)h, 1, 500.0f));

	return true;
}

void EffekseerManager::Update()
{
	if (!m_pEfkManager) { return; }

	// �G�t�F�N�g�̍X�V
	UpdateEffect();
	// �J���������݂���ꍇ�X�V
	UpdateEfkCameraMatrix();
}

void EffekseerManager::Draw()
{
	if (!m_pEfkManager || !m_pEfkRenderer ||
		!m_pEfkMemoryPool || !m_pEfkCmdList) {
		return;
	}

	// �����_�[�^�[�Q�b�g�I��
	m_pEfkMemoryPool->NewFrame();

	// �R�}���h���X�g�J�n
	EffekseerRendererDX12::BeginCommandList(m_pEfkCmdList, GraphicsDevice::GetInstance().GetCmdList());

	// �`�揈��
	m_pEfkRenderer->BeginRendering();
	m_pEfkManager->Draw();
	m_pEfkRenderer->EndRendering();

	// �R�}���h���X�g�I��
	EffekseerRendererDX12::EndCommandList(m_pEfkCmdList);
}

std::shared_ptr<EffekseerObject> EffekseerManager::Play(const std::string& fileName, const Math::Vector3& pos, bool isLoop)
{
	EffectInfo info;
	info.FileName = fileName;
	info.Pos = pos;
	info.Scale = 1.0f;
	info.Speed = 1.0f;
	info.IsLoop = isLoop;

	return Play(info);
}

void EffekseerManager::StopAllEffect()
{
	if (!m_pEfkManager) { return; }

	// ���[�v���̃G�t�F�N�g�̓��[�v�I��
	for (auto&& efk : m_omEffects)
	{
		if (!efk.second->IsLoop()) { continue; }
		efk.second->SetLoop(false);
	}

	// �S�ẴG�t�F�N�g�Đ����~
	m_pEfkManager->StopAllEffects();
}

void EffekseerManager::StopEffect(const std::string& name)
{
	// �w�肳�ꂽ�G�t�F�N�g�̍Đ����~
	auto it = m_omEffects.find(name);

	if (it == m_omEffects.end()) { return; }

	if (it->second->IsLoop())
	{
		it->second->SetLoop(false);
		return;
	}

	m_pEfkManager->StopEffect(it->second->GetHandle());
}

void EffekseerManager::SetPos(const std::string& name, const Math::Vector3& pos)
{
	// �G�t�F�N�g�̍��W�ݒ�
	auto it = m_omEffects.find(name);
	if (it == m_omEffects.end()) { return; }

	Effekseer::Vector3D efkPos = ConvertToEfkVec3D(pos);
	
	m_pEfkManager->SetLocation(it->second->GetHandle(), efkPos);
}

void EffekseerManager::SetRotation(const std::string& name, const Math::Vector3& axis, const float angle)
{
	// �G�t�F�N�g�̉�]�ݒ�
	auto it = m_omEffects.find(name);
	if (it == m_omEffects.end()) { return; }

	Effekseer::Vector3D efkAxis = ConvertToEfkVec3D(axis);

	m_pEfkManager->SetRotation(it->second->GetHandle(), efkAxis,angle);
}

void EffekseerManager::SetMatrix(const std::string& name, const Math::Matrix& mWorld)
{
	// �G�t�F�N�g�̍s��ݒ�
	auto it = m_omEffects.find(name);
	if (it == m_omEffects.end()) { return; }

	Effekseer::Matrix43 mEfk = {};

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mEfk.Value[i][j] = mWorld.m[i][j];
		}
	}

	m_pEfkManager->SetMatrix(it->second->GetHandle(), mEfk);
}

void EffekseerManager::SetScale(const std::string& name, const Math::Vector3& scale)
{
	// �G�t�F�N�g�̃X�P�[���ݒ�(�ʂɐݒ�\)
	auto it = m_omEffects.find(name);
	if (it == m_omEffects.end()) { return; }

	float x = scale.x;
	float y = scale.y;
	float z = scale.z;

	m_pEfkManager->SetScale(it->second->GetHandle(), x, y, z);
}

void EffekseerManager::SetScale(const std::string& name, const float scale)
{
	// �G�t�F�N�g�̃X�P�[���ݒ�(�S�ē��䗦�Őݒ�)
	auto it = m_omEffects.find(name);
	if (it == m_omEffects.end()) { return; }

	m_pEfkManager->SetScale(it->second->GetHandle(), scale, scale, scale);
}

void EffekseerManager::SetSpeed(const std::string& name, const float speed)
{
	// �G�t�F�N�g�̃X�s�[�h�ݒ�
	auto it = m_omEffects.find(name);
	if (it == m_omEffects.end()) { return; }

	m_pEfkManager->SetSpeed(it->second->GetHandle(), speed);
}

void EffekseerManager::SetPause(const std::string& name, const bool isPause)
{
	// �G�t�F�N�g�̈ꎞ��~�ݒ�
	auto it = m_omEffects.find(name);
	if (it == m_omEffects.end()) { return; }

	m_pEfkManager->SetPaused(it->second->GetHandle(), isPause);
}

const bool EffekseerManager::IsPlaying(const std::string& name) const
{
	// �G�t�F�N�g���Đ�����
	auto it = m_omEffects.find(name);
	if (it == m_omEffects.end()) { return false; }

	return it->second->IsPlaying();
}

void EffekseerManager::Release()
{
	StopAllEffect();

	m_omEffects.clear();

	m_isPause = false;
}

std::shared_ptr<EffekseerObject> EffekseerManager::Play(const EffectInfo& info)
{
	// ���W��Effekseer���W�ɕϊ�
	Effekseer::Vector3D efkPos = ConvertToEfkVec3D(info.Pos);

	Effekseer::Handle handle = 0;

	// �G�t�F�N�g���X�g�ɓo�^�ς݂�
	auto it = m_omEffects.find(info.FileName);
	if (it != m_omEffects.end())
	{
		handle = m_pEfkManager->Play(it->second->GetEffect(), efkPos);
		it->second->SetHandle(handle);
		it->second->SetEffectInfo(info);
		SetScale(info.FileName, info.Scale);
		SetSpeed(info.FileName, info.Speed);

		return it->second;
	}

	// �o�^����Ă��Ȃ��ꍇ�V�K�쐬
	std::string fileName = EffectPath + info.FileName;

	auto effect = Effekseer::Effect::Create(m_pEfkManager,
		(const EFK_CHAR*)sjis_to_wide(fileName).c_str(), info.Scale);
	if (!effect)
	{
		assert(0 && "�G�t�F�N�g�V�K�쐬���s");
		return nullptr;
	}

	std::shared_ptr<EffekseerObject> spEfkObj = std::make_shared<EffekseerObject>();

	handle = m_pEfkManager->Play(effect, efkPos);
	m_pEfkManager->SetSpeed(handle, info.Speed);

	spEfkObj->SetParentManager(m_pEfkManager);
	spEfkObj->SetEffect(effect);
	spEfkObj->SetHandle(handle);
	spEfkObj->SetEffectInfo(info);

	// �G�t�F�N�g���X�g�ɃG�t�F�N�g��o�^
	m_omEffects[info.FileName] = spEfkObj;

	return spEfkObj;
}

std::shared_ptr<EffekseerObject> EffekseerManager::Play(const std::shared_ptr<EffekseerObject> spEfkObj)
{
	return Play(spEfkObj->GetEffectInfo());
}

void EffekseerManager::UpdateEffect()
{
	if (m_isPause) { return; }

	// �G�t�F�N�g�̍X�V
	m_pEfkManager->Update();

	auto it = m_omEffects.begin();

	m_pEfkManager->BeginUpdate();

	// ���[�v�Đ����̃G�t�F�N�g�X�V
	while (it != m_omEffects.end())
	{
		bool isLoop = it->second->IsLoop();

		if (!isLoop)
		{
			++it;
			continue;
		}

		auto handle = it->second->GetHandle();

		if (m_pEfkManager->GetInstanceCount(handle) == 0)
		{
			Play(it->second);
		}

		++it;
	}

	m_pEfkManager->EndUpdate();
}

void EffekseerManager::UpdateEfkCameraMatrix()
{
	std::shared_ptr<Camera> spCamera = m_wpCamera.lock();
	if (!spCamera) { return; }

	// �J��������View,Proj���擾���ݒ�
	Math::Matrix mView = spCamera->GetCameraMatrix().Invert();
	Math::Matrix mProj = spCamera->GetProjMatrix();
	Effekseer::Matrix44 mEfkView;
	Effekseer::Matrix44 mEfkProj;

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

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
// EffekseerObject
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
bool EffekseerObject::IsPlaying()
{
	if (!m_parentManager) { return false; }

	return m_parentManager->GetInstanceCount(m_handle) != 0;
}

void EffekseerObject::SetPos(const Math::Vector3& pos)
{
	m_info.Pos = pos;

	EffekseerManager::GetInstance().SetPos(m_info.FileName, pos);
}

void EffekseerObject::SetScale(const float scale)
{
	m_info.Scale = scale;

	EffekseerManager::GetInstance().SetScale(m_info.FileName, scale);
}

void EffekseerObject::SetSpeed(const float speed)
{
	m_info.Speed = speed;

	EffekseerManager::GetInstance().SetSpeed(m_info.FileName, speed);
}

void EffekseerObject::SetMatrix(const Math::Matrix& mWorld) const
{
	EffekseerManager::GetInstance().SetMatrix(m_info.FileName, mWorld);
}

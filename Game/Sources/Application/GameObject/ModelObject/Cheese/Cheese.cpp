#include "Cheese.h"
#include "../../../Scene/SceneManager.h"

void Cheese::Draw()
{
	if (!SceneManager::GetInstance().GetGoalFlg())
	{
		if (!m_spModel) { return; }
		ShaderManager::GetInstance().m_modelShader.SetLimLightEnable(true);
		ShaderManager::GetInstance().m_modelShader.SetLimLight(2, { 1,1,0.8f });
		ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
	}
}

void Cheese::Update()
{
	if (!m_spModel) { return; }

	if (SceneManager::GetInstance().GetGoalFlg())
	{
		m_upCollider->SetEnableAll(false);
		m_spSound->Stop();
		EffekseerManager::GetInstance().StopEffect("Shine.efk");
		m_isExpired = true;
	}
	else
	{
		if (!EffekseerManager::GetInstance().IsPlaying("Shine.efk"))
		{
			auto effect = EffekseerManager::GetInstance().Play("Shine.efk", GetPos());
		}

		if (m_spSound->IsPlaying())
		{
			m_spSound->SetPos(GetPos());
			m_spSound->SetVolume(SceneManager::GetInstance().GetSEVolume());
			m_spSound->SetCurveDistanceScaler();
		}
		else if (m_spSound->IsStopped())
		{
			m_spSound->Play();
			m_spSound->SetVolume(SceneManager::GetInstance().GetSEVolume());
		}
	}
}

void Cheese::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Cheese/Cheese.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Cheese", m_spModel, Collider::Type::Bump | Collider::Type::Goal);

	m_spSound = AudioManager::GetInstance().Play3D("Assets/Sounds/Shine.wav", GetPos(), true);
	m_spSound->SetVolume(0);
	m_spSound->Stop();
}
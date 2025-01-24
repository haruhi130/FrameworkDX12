#include "Cheese.h"
#include "../../../Scene/SceneManager.h"

void Cheese::Draw()
{
	if (!SceneManager::GetInstance().GetGoalFlg())
	{
		if (!m_spModel) { return; }
		ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
	}
}

void Cheese::Update()
{
	if (SceneManager::GetInstance().GetGoalFlg())
	{
		m_spSound->Stop();
		EffekseerManager::GetInstance().StopEffect("Shine.efk");
	}

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
}

void Cheese::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Cheese/Cheese.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Cheese", m_spModel, Collider::Type::Bump|Collider::Type::Goal);

	m_spSound = AudioManager::GetInstance().Play3D("Assets/Sounds/get.wav",GetPos(), true);
}
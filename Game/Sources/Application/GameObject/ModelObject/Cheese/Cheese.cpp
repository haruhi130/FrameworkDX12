#include "Cheese.h"
#include "../../../Scene/SceneManager.h"

void Cheese::Update()
{
	if (!EffekseerManager::GetInstance().IsPlaying("Shine.efk"))
	{
		auto effect = EffekseerManager::GetInstance().Play("Shine.efk", GetPos());
	}

	m_se->SetPos(GetPos());
	m_se->SetVolume(SceneManager::GetInstance().GetSEVolume());
	m_se->SetCurveDistanceScaler();
}

void Cheese::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Cheese/Cheese.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Cheese", m_spModel, Collider::Type::Goal|Collider::Type::Bump);

	m_se = AudioManager::GetInstance().Play3D("Assets/Sounds/get.wav",GetPos(), true);
	m_se->SetVolume(SceneManager::GetInstance().GetSEVolume());
}
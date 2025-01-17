#include "TeaSet.h"
#include "../../../Scene/SceneManager.h"

void TeaSet::Update()
{
	m_se->SetVolume(SceneManager::GetInstance().GetSEVolume());
	m_se->SetCurveDistanceScaler();
}

void TeaSet::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/TeaSet/Tea_set.gltf"));
	}

	//Math::Matrix mScale = Math::Matrix::CreateScale(1.8f);
	//Math::Matrix mTrans = Math::Matrix::CreateTranslation(0.0f, 3.1f, 25.0f);
	//m_mWorld = mScale * mTrans;

	//m_upCollider = std::make_unique<Collider>();
	//m_upCollider->RegisterCollisionShape("TeaSet", m_spModel, Collider::Type::Goal);

	m_se = AudioManager::GetInstance().Play3D("Assets/Sounds/get.wav", GetPos(), true);
}

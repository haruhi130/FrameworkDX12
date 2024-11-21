#include "Cheese.h"
#include "../../../Scene/SceneManager.h"

void Cheese::Draw()
{
	if (!m_spModel) { return; }
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Cheese::DrawShadow()
{
	if (!m_spModel) { return; }

	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Cheese::Update()
{
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

	Math::Matrix mScale = Math::Matrix::CreateScale(1.0f);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(45.0f));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(0.1f, 3.1f, 25.1f);
	m_mWorld = mScale * mRot * mTrans;

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Cheese", m_spModel, Collider::Type::Goal);

	m_se = AudioManager::GetInstance().Play3D("Assets/Sounds/get.wav",GetPos(), true);
}
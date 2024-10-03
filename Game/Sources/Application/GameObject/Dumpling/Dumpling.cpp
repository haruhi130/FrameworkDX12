#include "Dumpling.h"

void Dumpling::Update()
{
}

void Dumpling::Draw()
{
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Dumpling::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Dumpling/Dumpling.gltf"));
	}

	Math::Matrix mScale = Math::Matrix::CreateScale(5);
	Math::Matrix mRot = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(0, 0.5, 0);
	m_mWorld = mScale * mRot * mTrans;

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Dump", m_spModel, Collider::Type::Event);
}

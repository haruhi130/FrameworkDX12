#include "Cheese.h"

void Cheese::Draw()
{
	if (!m_spModel) { return; }
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Cheese::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Cheese/Cheese.gltf"));
	}

	Math::Matrix mScale = Math::Matrix::CreateScale(0.2f);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(45.0f));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(15.0f, 3.0f, 0);
	m_mWorld = mScale * mRot * mTrans;

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Cheese", m_spModel, Collider::Type::Goal);
}
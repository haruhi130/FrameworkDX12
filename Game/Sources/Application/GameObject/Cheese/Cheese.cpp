#include "Cheese.h"

void Cheese::Update()
{
}

void Cheese::Draw()
{
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Cheese::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Cheese/Cheese.gltf"));
	}

	Math::Matrix mScale = Math::Matrix::CreateScale(0.5f);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(15, 0, 0);
	m_mWorld = mScale * mTrans;

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Cheese", m_spModel, Collider::Type::Event | Collider::Type::Goal);
}

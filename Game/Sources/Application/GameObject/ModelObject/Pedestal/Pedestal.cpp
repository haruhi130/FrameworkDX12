#include "Pedestal.h"

void Pedestal::Draw()
{
	if (!m_spModel) { return; }

	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Pedestal::DrawShadow()
{
	if (!m_spModel) { return; }

	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Pedestal::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Building/Japanese_Farmers_House.gltf"));
	}

	Math::Matrix mScale = Math::Matrix::CreateScale(1.5f);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_pos);

	m_mWorld = mScale * mTrans;

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Pedestal", m_spModel, Collider::Type::Event | Collider::Type::Ground | Collider::Type::Bump);
}
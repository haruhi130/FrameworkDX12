#include "Box.h"

void Box::Draw()
{
	if (!m_spModel) { return; }

	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Box::DrawShadow()
{
	if (!m_spModel) { return; }

	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Box::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Box/Box.gltf"));
	}

	Math::Matrix mScale = Math::Matrix::CreateScale(2);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = mScale * mTrans;

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Box", m_spModel, Collider::Type::Event | Collider::Type::Ground | Collider::Type::Bump);
}

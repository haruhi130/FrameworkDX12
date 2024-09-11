#include "Ground.h"
#include "../../Application.h"

void Ground::Draw()
{
	Application::GetInstance().GetShader().DrawModel(*m_spModel, m_mWorld);
}

void Ground::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData("Assets/Models/Ground/Ground.gltf");
	
		m_upCollider = std::make_unique<Collider>();
		m_upCollider->RegisterCollisionShape(
			"Ground", m_spModel, Collider::Type::Bump | Collider::Type::Ground);
	}

	Math::Matrix mScale = Math::Matrix::CreateScale(100, 1, 100);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(0, 0, 0);
	m_mWorld = mScale * mTrans;
}

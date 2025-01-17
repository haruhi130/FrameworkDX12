#include "Pedestal.h"

void Pedestal::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Pedestal/Pedestal.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Pedestal", m_spModel, Collider::Type::Event | Collider::Type::Ground | Collider::Type::Bump);
}
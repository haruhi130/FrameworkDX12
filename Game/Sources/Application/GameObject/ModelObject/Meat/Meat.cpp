#include "Meat.h"

void Meat::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Meat/Meat.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Meat", m_spModel, Collider::Type::Event | Collider::Type::Ground | Collider::Type::Bump);
}

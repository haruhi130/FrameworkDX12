#include "Dumpling.h"

void Dumpling::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.
			GetData("Assets/Models/Dumpling/Dumpling.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Dumpling", m_spModel,
		Collider::Type::Ground | Collider::Type::Bump | Collider::Type::Event);
}

#include "CirclePedestal.h"

void CirclePedestal::Update()
{
	if (!m_spModel) { return; }

	ShaderManager::GetInstance().AddSpotLight({ 1.0f,1.0f,0.8f }, 20, GetPos() + Math::Vector3(0, 15, 0),
		90, Math::Vector3::Down);
}

void CirclePedestal::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/CirclePedestal/CirclePedestal.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("CirclePedestal", m_spModel, Collider::Type::Ground | Collider::Type::Bump | Collider::Type::Event);
}

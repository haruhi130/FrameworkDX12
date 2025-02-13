#include "Pedestal.h"

void Pedestal::Update()
{
	if (!m_spModel) { return; }

	ShaderManager::GetInstance().AddPointLight({ 1.0f,1.0f,0.8f }, 8, GetPos() + Math::Vector3(0, 5
		, 0), true);
}

void Pedestal::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/Pedestal/Pedestal.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Pedestal", m_spModel, Collider::Type::Ground | Collider::Type::Bump | Collider::Type::Event);
}
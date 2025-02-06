#include "Trex.h"

void Trex::Update()
{
	auto pos = GetPos() + Math::Vector3(0, 8, 0);
	ShaderManager::GetInstance().AddPointLight({ 1,1,1 }, 10, pos, 1);
}

void Trex::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.
			GetData("Assets/Models/Trex/Trex.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("rex", m_spModel, Collider::Type::Ground | Collider::Type::Bump | Collider::Type::Event);
}

#include "Cow.h"

void Cow::Update()
{
	if (!m_spModel) { return; }

	ShaderManager::GetInstance().AddSpotLight({ 1.0f,1.0f,0.8f }, 18, GetPos() + Math::Vector3(0, 15, 0),
		90, Math::Vector3::Down);
}

void Cow::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.
			GetData("Assets/Models/Cow/Cow.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Cow", m_spModel, Collider::Type::Ground | Collider::Type::Bump | Collider::Type::Event);
}

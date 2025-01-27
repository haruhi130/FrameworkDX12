#include "Stage.h"

void Stage::DrawShadow()
{
}

void Stage::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.
			GetData("Assets/Models/Stage/Stage_ver2.gltf"));
	}

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape(
		"Stage", m_spModel, Collider::Type::Ground | Collider::Type::Bump);
}

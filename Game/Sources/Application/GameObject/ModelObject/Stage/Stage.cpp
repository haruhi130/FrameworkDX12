#include "Stage.h"

void Stage::Draw()
{
	if (!m_spModel) { return; }
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Stage::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.
			GetData("Assets/Models/Stage/Stage.gltf"));

		m_upCollider = std::make_unique<Collider>();
		m_upCollider->RegisterCollisionShape(
			"Stage", m_spModel, Collider::Type::Ground | Collider::Type::Bump);
	}

	Math::Matrix mScale = Math::Matrix::CreateScale(2,0.5,2);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(0, 0, 0);
	m_mWorld = mScale * mTrans;
}

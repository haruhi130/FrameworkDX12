#include "Sky.h"

void Sky::Update()
{
	if (!m_spModel) { return; }

	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		targetMat = Math::Matrix::CreateTranslation(m_wpTarget.lock()->GetPos());
	}

	Math::Matrix mScale = Math::Matrix::CreateScale(m_scale);
	m_mWorld = mScale * targetMat;
}

void Sky::Draw()
{
	if (!m_spModel) { return; }
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld, false);
}

void Sky::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/SkySphere/SkySphere.gltf"));
	}

	m_scale = 100.0f;
}

#include "Sky.h"

void Sky::Update()
{
	if (!m_spModel) { return; }

	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		targetMat = Math::Matrix::CreateTranslation(m_wpTarget.lock()->GetPos());
	}

	m_mLocal = Math::Matrix::CreateTranslation(m_pos);

	Math::Matrix mScale = Math::Matrix::CreateScale(100);
	m_mWorld = m_mLocal * mScale * targetMat;
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

	m_mLocal = Math::Matrix::CreateTranslation(m_pos);

	Math::Matrix mScale = Math::Matrix::CreateScale(200);
	m_mWorld = mScale;
}

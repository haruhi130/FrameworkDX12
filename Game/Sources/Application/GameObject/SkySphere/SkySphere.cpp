#include "SkySphere.h"

void SkySphere::Update()
{
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		targetMat = Math::Matrix::CreateTranslation(m_wpTarget.lock()->GetPos());
	}

	m_mLocal = Math::Matrix::CreateTranslation(m_localPos);

	Math::Matrix mScale = Math::Matrix::CreateScale(100);
	m_mWorld = m_mLocal * mScale * targetMat;
}

void SkySphere::Draw()
{
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void SkySphere::SetTarget(const std::shared_ptr<BaseObject>& target)
{
	if (!target) return;
	m_wpTarget = target;
}

void SkySphere::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.GetData("Assets/Models/SkySphere/SkySphere.gltf"));
	}

	m_localPos = { 0,0,0 };
	m_mLocal = Math::Matrix::CreateTranslation(m_localPos);

	Math::Matrix mScale = Math::Matrix::CreateScale(100);
	m_mWorld = mScale;

}

#include "Ground.h"
#include "../../Application.h"

void Ground::Draw()
{
	Application::GetInstance().GetShader().DrawModel(*m_spModel, m_mWorld);
}

void Ground::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData("Assets/Models/Ground/Ground.gltf");
	}

	Math::Matrix mScale = Math::Matrix::CreateScale(100, 1, 100);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(0, 0, 0);
	m_mWorld = mScale * mTrans;
}

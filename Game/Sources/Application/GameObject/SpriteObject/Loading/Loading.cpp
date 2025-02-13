#include "Loading.h"
#include "../../../Scene/SceneManager.h"

void Loading::DrawSprite()
{
	if (!m_spTex) { return; }

	if (m_isRot)
	{
		m_rotateVec.z += 5.0f;
		auto rot = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_rotateVec.z));
		auto trans = Math::Matrix::CreateTranslation({ m_pos.x,m_pos.y,0 });
		m_mWorld = rot * trans;
	}

	if (SceneManager::GetInstance().GetIsLoading())
	{
		Math::Color color = { 1.0f,1.0f,1.0f,1.0f };

		ShaderManager::GetInstance().m_spriteShader.SetMatrix(m_mWorld);
		ShaderManager::GetInstance().m_spriteShader.DrawTexture(m_spTex.get(), m_spMesh.get(), color);
	}
}

void Loading::Update()
{
	if (!m_spTex) { return; }

	if (!SceneManager::GetInstance().GetIsLoading())
	{
		m_isExpired = true;
	}
}

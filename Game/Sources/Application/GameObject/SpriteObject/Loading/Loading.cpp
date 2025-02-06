#include "Loading.h"
#include "../../../Scene/SceneManager.h"

void Loading::DrawSprite()
{
	if (!m_spTex) { return; }

	if (SceneManager::GetInstance().GetIsLoading())
	{
		Math::Color color = { 1.0f,1.0f,1.0f,m_alpha };

		ShaderManager::GetInstance().m_spriteShader.SetMatrix(m_mWorld);
		ShaderManager::GetInstance().m_spriteShader.DrawTexture(m_spTex.get(), m_spMesh.get(), color);
	}
}

void Loading::Update()
{
	if (!SceneManager::GetInstance().GetIsLoading())
	{
		m_isExpired = true;
	}

	SpriteObject::Update();
}

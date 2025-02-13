#include "SpriteObject.h"

void SpriteObject::Update()
{
	if (!m_spTex) { return; }

	if (m_isAlphaFluct)
	{
		m_alpha += m_fluctValue;
		if (m_alpha > 1.0f)
		{
			m_fluctValue *= -1.0f;
		}
		if (m_alpha < 0.5f)
		{
			m_fluctValue *= -1.0f;
		}
	}
}

void SpriteObject::DrawSprite()
{
	if (!m_spTex) { return; }

	Math::Color color = { 1.0f,1.0f,1.0f,m_alpha };

	ShaderManager::GetInstance().m_spriteShader.SetMatrix(m_mWorld);
	ShaderManager::GetInstance().m_spriteShader.DrawTexture(m_spTex.get(), m_spMesh.get(), color);
}

void SpriteObject::Init()
{
	if (!m_spTex)
	{
		m_spTex = std::make_shared<Texture>();
	}

	if (!m_spMesh)
	{
		m_spMesh = std::make_shared<Mesh>();
	}

	m_pos = {};
	m_rect = {};
	m_mWorld = Math::Matrix::Identity;
}

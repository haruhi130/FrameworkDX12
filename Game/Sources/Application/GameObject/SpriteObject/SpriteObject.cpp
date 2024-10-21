#include "SpriteObject.h"

void SpriteObject::Update()
{
}

void SpriteObject::DrawSprite()
{
	ShaderManager::GetInstance().m_spriteShader.SetMatrix(m_mWorld);
	ShaderManager::GetInstance().m_spriteShader.DrawTexture(m_spTex.get());
}

void SpriteObject::Init()
{
	if (!m_spTex)
	{
		m_spTex = std::make_shared<Texture>();
	}

	m_pos = {};
	m_rect = {};
	m_mWorld = Math::Matrix::Identity;
}

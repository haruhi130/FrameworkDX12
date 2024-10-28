#include "SpriteObject.h"

void SpriteObject::Update()
{
}

void SpriteObject::DrawSprite()
{
	ShaderManager::GetInstance().m_spriteShader.SetMatrix(m_mWorld);
	ShaderManager::GetInstance().m_spriteShader.DrawTexture(m_spTex.get(),m_spMesh.get());
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

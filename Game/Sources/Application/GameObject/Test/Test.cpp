#include "Test.h"

void Test::DrawSprite()
{
	ShaderManager::GetInstance().m_spriteShader.SetMatrix(m_mWorld);
	ShaderManager::GetInstance().m_spriteShader.DrawTex(m_spTex.get(), m_pos.x, m_pos.y, &m_rect);
}

void Test::Init()
{
	if (!m_spTex)
	{
		m_spTex = std::make_shared<Texture>();
		m_spTex->Load("Assets/Textures/back.png");
	}
}

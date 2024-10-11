#include "Test.h"

void Test::DrawSprite()
{
	ShaderManager::GetInstance().m_spriteShader.SetMatrix(m_mWorld);
	ShaderManager::GetInstance().m_spriteShader.DrawTex(m_spTex.get(), (int)m_pos.x, (int)m_pos.y,640,360, &m_rect);
}

void Test::Init()
{
	if (!m_spTex)
	{
		m_spTex = std::make_shared<Texture>();
		m_spTex = Assets::GetInstance().m_textures.GetData("Assets/Textures/back.png");
		//m_spTex->Load("Assets/Textures/back.png");
	}
}

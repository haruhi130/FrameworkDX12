#pragma once

class SpriteObject : public BaseObject
{
public:
	SpriteObject() { Init(); }
	~SpriteObject()override{}

	void Update()override;
	void DrawSprite()override;

	void SetTexture(std::string name)
	{ 
		if (!m_spTex)
		{
			m_spTex = std::make_shared<Texture>();
		}
		m_spTex = Assets::GetInstance().m_textures.GetData(name);

		if (!m_spMesh)
		{
			m_spMesh = std::make_shared<Mesh>();
		}
		m_spMesh->Create(*m_spTex, m_pos, &m_rect);
	}

	void SetPos(Math::Vector2 pos)
	{
		m_pos = pos;
	}

	void SetRectangle(Math::Rectangle rect)
	{
		m_rect = rect;
	}

private:
	void Init()override;

	std::shared_ptr<Texture> m_spTex = nullptr;
	std::shared_ptr<Mesh> m_spMesh = nullptr;

	Math::Vector2 m_pos;
	Math::Rectangle m_rect;
};
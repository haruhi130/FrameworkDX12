#pragma once

class SpriteObject : public BaseObject
{
public:
	SpriteObject() { Init(); }
	virtual ~SpriteObject()override{}

	virtual void Update()override;
	virtual void DrawSprite()override;

	virtual void SetTexture(const std::string& name)
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
		m_spMesh->Create(*m_spTex, 0,0, &m_rect);
	}

	virtual void SetPos(const Math::Vector2& pos)
	{
		m_pos = pos;
		m_mWorld.Translation({ m_pos.x,m_pos.y,0 });
	}

	virtual void SetRectangle(const Math::Rectangle& rect)
	{
		m_rect = rect;
	}

	virtual void SetAlphaFluct(bool isAlphaFluct = true, float fluctValue = 0.01f)
	{
		m_isAlphaFluct = isAlphaFluct;
		m_fluctValue = fluctValue;
	}

protected:
	virtual void Init()override;

	std::shared_ptr<Texture> m_spTex = nullptr;
	std::shared_ptr<Mesh> m_spMesh = nullptr;

	Math::Vector2 m_pos;
	Math::Rectangle m_rect;

	bool m_isAlphaFluct = false;
	float m_alpha = 1.0f;
	float m_fluctValue = 0.01f;
};
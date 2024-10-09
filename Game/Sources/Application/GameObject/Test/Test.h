#pragma once

class Test : public BaseObject
{
public:
	Test() { Init(); }
	~Test()override{}

	void DrawSprite()override;

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

	Math::Vector2 m_pos;
	Math::Rectangle m_rect;

	std::shared_ptr<Texture> m_spTex = nullptr;
};
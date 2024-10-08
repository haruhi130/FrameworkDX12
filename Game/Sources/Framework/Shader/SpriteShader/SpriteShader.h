#pragma once

#include "../ShaderBase.h"

class SpriteShader : public ShaderBase
{
public:
	struct Vertex
	{
		Vertex(Math::Vector3 position, Math::Vector2 uv)
			:Pos(position), UV(uv) {}

		Math::Vector3 Pos;
		Math::Vector2 UV;
	};

	bool Init()override;

	void Begin();

	void DrawTex(const Texture* tex, int x, int y, int w, int h, const Math::Rectangle* srcRect = nullptr, const Math::Color& color = {1,1,1,1}, const Math::Vector2& pivot = {0.5f,0.5f});

	inline void SetMatrix(const Math::Matrix& m)
	{ m_cbSprite.mTransform = m; }

private:
	ConstantBufferData::Sprite m_cbSprite;

};
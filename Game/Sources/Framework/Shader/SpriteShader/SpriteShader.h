#pragma once

#include "../ShaderBase.h"

class SpriteShader : public ShaderBase
{
public:
	struct SpriteVertex
	{
		SpriteVertex(Math::Vector3 position, Math::Vector2 uv)
			:Pos(position), UV(uv) {}

		Math::Vector3 Pos;
		Math::Vector2 UV;
	};

	bool Init()override;

	void Begin(int w = 1280, int h = 720);

	void DrawTex(const Texture* tex, int x, int y, int w, int h, const Math::Rectangle* srcRect = nullptr, const Math::Color& color = {1,1,1,1}, const Math::Vector2& pivot = {0.5f,0.5f});

	void DrawTex(const Texture* tex, int x, int y, const Math::Rectangle* srcRect = nullptr, const Math::Color& color = { 1,1,1,1 }, const Math::Vector2& pivot = { 0.5f,0.5f })
	{
		if (!tex) { return; }
		DrawTex(tex, x, y, tex->GetInfo().Width, tex->GetInfo().Height, srcRect, color, pivot);
	}

	inline void SetMatrix(const Math::Matrix& m)
	{ m_cbSprite.mTransform = m; }

private:
	ConstantBufferData::Sprite m_cbSprite;
	ConstantBufferData::Projection m_cbProj;

	std::shared_ptr<Mesh> m_spMesh = nullptr;

	Math::Matrix m_mProj2D;
};
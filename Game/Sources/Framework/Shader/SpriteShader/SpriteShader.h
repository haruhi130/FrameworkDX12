#pragma once

#include "../ShaderBase.h"

class SpriteShader : public ShaderBase
{
public:

	bool Init()override;

	void Begin(int w = windowWidth, int h = windowHeight);

	void DrawTexture(const Texture* tex,const Math::Color& color = { 1,1,1,1 });

	inline void SetMatrix(const Math::Matrix& m)
	{ 
		m_cbSprite.mTransform = m;
	}

private:
	ConstantBufferData::SpriteInfo m_cbSprite;
	ConstantBufferData::ProjectionInfo m_cbProj;

};
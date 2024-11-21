#pragma once
#include "../ShaderBase.h"

class SpriteShader : public ShaderBase
{
public:
	// 初期化
	bool Init()override;

	// シェーダー設定
	void Begin(int w = window_width, int h = window_height);

	// 画像描画
	void DrawTexture(const Texture* tex,const Mesh* mesh,const Math::Color& color = { 1,1,1,1 });

	// 行列設定
	inline void SetMatrix(const Math::Matrix& m)
	{  m_cbSprite.mTransform = m; }

private:
	// 定数バッファ
	ConstantBufferData::SpriteInfo m_cbSprite;
	ConstantBufferData::ProjectionInfo m_cbProj;
};
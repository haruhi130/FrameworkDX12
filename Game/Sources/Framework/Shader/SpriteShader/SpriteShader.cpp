#include "SpriteShader.h"

bool SpriteShader::Init()
{
	// シェーダーに渡す情報設定
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD};
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Create(L"SpriteShader/SpriteShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::SRV});

	return true;
}

void SpriteShader::Begin()
{
	ShaderBase::Begin();
}

void SpriteShader::DrawTex(const Texture* tex, int x, int y, int w, int h, const Math::Rectangle* srcRect, const Math::Color& color, const Math::Vector2& pivot)
{
	if (tex == nullptr) { return; }

	tex->Set(m_cbvCount + tex->GetSRVNumber());

	m_cbSprite.Color = color;
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(1, m_cbSprite);

	Math::Vector2 uvMin = { 0,0 };
	Math::Vector2 uvMax = { 1,1 };

	if (srcRect)
	{
		uvMin.x = srcRect->x / (float)tex->GetInfo().Width;
		uvMin.y = srcRect->y / (float)tex->GetInfo().Height;

		uvMax.x = (srcRect->x + srcRect->width) / (float)tex->GetInfo().Width;
		uvMax.y = (srcRect->y + srcRect->height) / (float)tex->GetInfo().Height;
	}

	float x1 = (float)x;
	float y1 = (float)y;
	float x2 = (float)(x + w);
	float y2 = (float)(y + h);

	x1 -= pivot.x * w;
	x2 -= pivot.x * w;
	y1 -= pivot.y * h;
	y2 -= pivot.y * h;

	Vertex vertex[] = {
		{ {x1, y1, 0},	{uvMin.x, uvMax.y} },
		{ {x1, y2, 0},	{uvMin.x, uvMin.y} },
		{ {x2, y1, 0},	{uvMax.x, uvMax.y} },
		{ {x2, y2, 0},	{uvMax.x, uvMin.y} }
	};

}

#include "SpriteShader.h"

bool SpriteShader::Init()
{
	// シェーダーに渡す情報設定
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Create(L"SpriteShader/SpriteShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });

	return true;
}

void SpriteShader::Begin(int w, int h)
{
	ShaderBase::Begin(w, h);

	m_cbProj.mProj = DirectX::XMMatrixOrthographicLH((float)w, (float)h, 0, 1);

	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(2, m_cbProj);
}

void SpriteShader::DrawTexture(const Texture* tex,const Math::Color& color)
{
	if (tex == nullptr) { return; }

	tex->Set(m_cbvCount);

	m_cbSprite.Color = color;
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(1, m_cbSprite);

	tex->SetToDevice();
	GraphicsDevice::GetInstance().GetCmdList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

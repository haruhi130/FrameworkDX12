#include "SpriteShader.h"

bool SpriteShader::Init()
{
	ShaderBase::Init();

	// シェーダーに渡す情報設定
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };
	renderingSetting.IsDepth = false;

	// 2D描画シェーダー作成
	Create(m_spRootSignature, m_spPipeline, L"SpriteShader/SpriteShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::SRV });

	return true;
}

void SpriteShader::Begin(int w, int h)
{
	// DescriptorHeap設定
	GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

	// ルートシグネチャとパイプライン設定
	ShaderBase::Begin(m_spRootSignature, m_spPipeline, w, h);

	// 射影行列設定
	m_cbProj.mProj = DirectX::XMMatrixOrthographicLH((float)w, (float)h, 0, 1);

	// 射影行列情報転送
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(1, m_cbProj);
}

void SpriteShader::DrawTexture(const Texture* tex, const Mesh* mesh, const Math::Color& color)
{
	if (tex == nullptr) { return; }

	// 画像をシェーダーにセット
	tex->Set(m_cbvCount);

	// 色情報転送
	m_cbSprite.Color = color;
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(0, m_cbSprite);

	// 描画
	mesh->DrawIndexed();
}

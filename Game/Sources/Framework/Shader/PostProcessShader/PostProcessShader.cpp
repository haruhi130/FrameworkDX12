#include "PostProcessShader.h"

bool PostProcessShader::Init()
{
	ShaderBase::Init();

	// レンダーターゲット変更用リソース作成
	if (!m_RTChange.CreateRenderTarget()) { return false; }
	if (!m_RTChange.CreateRTTexture()) { return false; }

	// 深度テクスチャ作成
	if (!m_RTChange.CreateDepthTexture()) { return false; }
	
	// シェーダーに渡す情報設定
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };
	renderingSetting.IsDepth = false;

	// ポストプロセス描画シェーダー作成
	Create(m_spRootSignature, m_spPipeline, L"PostProcessShader/PostProcessShader"
		, renderingSetting, { RangeType::SRV });

	return true;
}

void PostProcessShader::Begin(int w, int h)
{
	// DescriptorHeap設定
	GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

	// ルートシグネチャとパイプライン設定
	ShaderBase::Begin(m_spRootSignature, m_spPipeline, w, h);

	// 変更したレンダーターゲット先に書き込んだ画像をシェーダーにセット
	m_RTChange.m_spRTTexture->SetToShader(m_cbvCount);
}

void PostProcessShader::PreDraw()
{
	// レンダーターゲット変更
	m_RTChange.ChangeRenderTarget();
}

void PostProcessShader::Draw()
{
	// 描画
	m_RTChange.Draw();
}

void PostProcessShader::PostProcess()
{
	// レンダーターゲット変更
	m_RTChange.UndoRenderTarget();
}

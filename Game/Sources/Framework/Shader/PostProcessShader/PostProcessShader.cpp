#include "PostProcessShader.h"

bool PostProcessShader::Init()
{
	ShaderBase::Init();

	if (!m_RTChange.CreateRenderTarget()) { return false; }
	if (!m_RTChange.CreateRTTexture()) { return false; }

	if (!m_RTChange.CreateDepthTexture()) { return false; }
	
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };
	renderingSetting.IsDepth = false;

	Create(m_spRootSignature, m_spPipeline, L"PostProcessShader/PostProcessShader"
		, renderingSetting, { RangeType::SRV });

	return true;
}

void PostProcessShader::Begin(int w, int h)
{
	ShaderBase::Begin(m_spRootSignature, m_spPipeline, w, h);
}

void PostProcessShader::PreDraw()
{
	m_RTChange.ChangeRenderTarget();
}

void PostProcessShader::Draw()
{
	m_RTChange.m_spRTTexture->Set(m_cbvCount);
	m_RTChange.Draw();
}

void PostProcessShader::PostProcess()
{
	m_RTChange.UndoRenderTarget();
}

#include "PostProcessShader.h"

bool PostProcessShader::Init()
{
	if (!m_change.CreateRenderTarget()) { return false; }
	if (!m_change.CreateRTTexture()) { return false; }
	
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts = 
	{ InputLayout::POSITION,InputLayout::TEXCOORD };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Create(L"PostProcessShader/PostProcessShader", renderingSetting,{ RangeType::SRV });
	
	return true;
}

void PostProcessShader::Begin(int w, int h)
{
	ShaderBase::Begin(w,h);
}

void PostProcessShader::PreDraw()
{
	m_change.ChangeRenderTarget();
}

void PostProcessShader::Draw()
{
	m_change.m_spRTTexture->Set(m_cbvCount);
	m_change.Draw();
}

void PostProcessShader::PostProcess()
{
	m_change.UndoRenderTarget();
}

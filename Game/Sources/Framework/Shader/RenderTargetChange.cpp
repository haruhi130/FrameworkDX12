#include "RenderTargetChange.h"

bool RenderTargetChange::CreateRenderTarget()
{
	// テクスチャからリソース作成
	if (!m_spRTTexture)
	{
		m_spRTTexture = std::make_shared<Texture>();
	}
	if (!m_spRTTexture->CreateResource())
	{
		return false;
	}

	// RTV作成
	m_rtvNum = GraphicsDevice::GetInstance().GetRTVHeap()->CreateRTV(m_spRTTexture->GetBuffer().Get());

	// SRV作成
	m_renderTargetSRVNumber = GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->CreateSRV(m_spRTTexture->GetBuffer().Get());

	return true;
}

void RenderTargetChange::ChangeRenderTarget() const
{
	// リソースバリア切り替え
	GraphicsDevice::GetInstance().SetResourceBarrier(m_spRTTexture->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// RTVハンドル取得
	auto rtvH = GraphicsDevice::GetInstance().GetRTVHeap()->GetCPUHandle(m_rtvNum);
	
	// DSVハンドル取得
	auto dsvH = GraphicsDevice::GetInstance().GetDSVHeap()->GetCPUHandle(
		GraphicsDevice::GetInstance().GetDepthStencil()->GetDSVNumber());

	// レンダーターゲット切り替え
	GraphicsDevice::GetInstance().GetCmdList()->OMSetRenderTargets(
		1, &rtvH, false, &dsvH);

	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	GraphicsDevice::GetInstance().GetCmdList()->ClearRenderTargetView(
		rtvH, clearColor, 0, nullptr);

	// 通常深度バッファクリア
	GraphicsDevice::GetInstance().GetDepthStencil()->ClearBuffer();
}

void RenderTargetChange::UndoRenderTarget() const
{
	// リソースバリア切り替え
	GraphicsDevice::GetInstance().SetResourceBarrier(m_spRTTexture->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

bool RenderTargetChange::CreateRTTexture()
{
	// レンダーターゲット用板ポリ作成
	if (!m_spRTTexture)
	{
		m_spRTTexture = std::make_shared<Texture>();
	}
	if (!m_spRTTexture->CreateRenderTarget())
	{
		return false;
	}

	return true;
}

bool RenderTargetChange::CreateDepthTexture()
{
	// 深度テクスチャ作成
	if (!m_spDepthTexture)
	{
		m_spDepthTexture = std::make_shared<Texture>();
	}
	if (!m_spDepthTexture->CreateDepthSRV())
	{
		return false;
	}

	return true;
}

void RenderTargetChange::Draw() const
{
	m_spRTTexture->SetToDevice();

	GraphicsDevice::GetInstance().GetCmdList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

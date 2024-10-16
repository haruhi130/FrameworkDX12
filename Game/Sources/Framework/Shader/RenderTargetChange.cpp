#include "RenderTargetChange.h"

bool RenderTargetChange::CreateRenderTarget()
{
	auto heapDesc = GraphicsDevice::GetInstance().GetRTVHeap()
		->GetHeap()->GetDesc();

	auto bbuff = GraphicsDevice::GetInstance().GetBackBuffers();
	auto resDesc = bbuff[0]->GetDesc();

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	float ClearColor[4] = { 0.5,0.5,0.5,1 };
	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	clearValue.Color[0] = ClearColor[0];
	clearValue.Color[1] = ClearColor[1];
	clearValue.Color[2] = ClearColor[2];
	clearValue.Color[3] = ClearColor[3];

	auto result = GraphicsDevice::GetInstance().GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
		, &clearValue, IID_PPV_ARGS(m_cpResource.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0 && "マルチパスレンダリング用リソース作成失敗");
		return false;
	}

	// RTVHeap作成
	heapDesc.NumDescriptors = 1;
	result = GraphicsDevice::GetInstance().GetDevice()->CreateDescriptorHeap(
		&heapDesc, IID_PPV_ARGS(m_cpRTVResource.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0 && "マルチパスレンダリング用レンダーターゲットビュー作成失敗");
		return false;
	}

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	auto handle = m_cpRTVResource->GetCPUDescriptorHandleForHeapStart();

	GraphicsDevice::GetInstance().GetDevice()->CreateRenderTargetView(
		m_cpResource.Get(), &rtvDesc, handle);
	
	// SRVHeap作成
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	result = GraphicsDevice::GetInstance().GetDevice()->CreateDescriptorHeap(
		&heapDesc, IID_PPV_ARGS(m_cpSRVResource.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0 && "マルチパスレンダリング用シェーダーリソースビュー作成失敗");
		return false;
	}

	handle = m_cpSRVResource->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = rtvDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	GraphicsDevice::GetInstance().GetDevice()->CreateShaderResourceView(
		m_cpResource.Get(), &srvDesc, handle);

	return true;
}

bool RenderTargetChange::ChangeRenderTarget()
{
	GraphicsDevice::GetInstance().SetResourceBarrier(m_cpResource.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	auto rtvPointer = m_cpRTVResource->GetCPUDescriptorHandleForHeapStart();
	auto dsvPointer = GraphicsDevice::GetInstance().GetDSVHeap()->GetHeap()->GetCPUDescriptorHandleForHeapStart();

	GraphicsDevice::GetInstance().GetCmdList()->OMSetRenderTargets(
		1, &rtvPointer, false, &dsvPointer);

	float clearColor[4] = { 0.5,0.5,0.5,1 };
	GraphicsDevice::GetInstance().GetCmdList()->ClearRenderTargetView(
	rtvPointer,clearColor,0,nullptr);

	GraphicsDevice::GetInstance().GetDepthStencil()->ClearBuffer();

	return true;
}

void RenderTargetChange::UndoRenderTarget()
{
	GraphicsDevice::GetInstance().SetResourceBarrier(m_cpResource.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

bool RenderTargetChange::CreateRTTexture()
{
	m_spRTTexture = std::make_shared<Texture>();
	if (!m_spRTTexture->CreateRenderTarget())
	{
		return false;
	}

	return true;
}

void RenderTargetChange::Draw()
{
	GraphicsDevice::GetInstance().GetCmdList()->SetDescriptorHeaps(1, m_cpSRVResource.GetAddressOf());
	auto handle = m_cpSRVResource->GetGPUDescriptorHandleForHeapStart();
	GraphicsDevice::GetInstance().GetCmdList()->SetGraphicsRootDescriptorTable(
	0,handle);

	m_spRTTexture->SetToDevice();
	
	GraphicsDevice::GetInstance().GetCmdList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

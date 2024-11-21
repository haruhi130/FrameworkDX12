#include "DSVHeap.h"

int DSVHeap::CreateDSV(ID3D12Resource* pBuffer, DXGI_FORMAT format)
{
	if (m_useCount < m_nextRegistNumber)
	{
		assert(0 && "確保済みのヒープ領域をオーバー");
		return 0;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_cpHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)m_nextRegistNumber * m_incrementSize;

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = format;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	GraphicsDevice::GetInstance().GetDevice()->CreateDepthStencilView(pBuffer, &dsvDesc, handle);

	return m_nextRegistNumber++;
}

#include "ConstantBufferAllocator.h"

void ConstantBufferAllocator::Create(CBVSRVUAVHeap* pHeap)
{
	m_pHeap = pHeap;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Width = (UINT64)((1 + 0xff) & ~0xff) * (int)m_pHeap->GetUseCount().x;

	auto result = GraphicsDevice::GetInstance().GetDevice()->
		CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_cpBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0 && "定数バッファアロケーター作成失敗");
		return;
	}

	m_cpBuffer->Map(0, nullptr, (void**)&m_pMappedBuffer);
}

void ConstantBufferAllocator::ResetCurrentUseNumber()
{
	m_currentNumber = 0;
}

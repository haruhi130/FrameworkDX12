#include "RTVHeap.h"

bool RTVHeap::Create()
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	auto result = GraphicsDevice::GetInstance().GetDevice()
		->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_cpRTVHeap.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

D3D12_CPU_DESCRIPTOR_HANDLE RTVHeap::GetCPUHandle()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_cpRTVHeap->
		GetCPUDescriptorHandleForHeapStart();
	return handle;
}

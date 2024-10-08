#pragma once

class DSVHeap : public BaseHeap<int>
{
public:
	DSVHeap(){}
	~DSVHeap()override{}

	// DSVì¬
	int CreateDSV(ID3D12Resource* pBuffer, DXGI_FORMAT format);
};
#pragma once

class DSVHeap :public BaseHeap<int>
{
public:
	DSVHeap(){}
	~DSVHeap(){}

	// DSVçÏê¨
	int CreateDSV(ID3D12Resource* pBuffer, DXGI_FORMAT format);
};